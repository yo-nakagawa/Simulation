/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "udp-ndn.h"
#include "ndn-packet.h"
#include "/home/nuc1/Simulation/cereal-1.3.0/include/cereal/archives/json.hpp"
#include <fstream>
#include "ns3/ipv4-interface.h"
#include "ns3/udp-socket-factory.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpNdnApplication");

NS_OBJECT_ENSURE_REGISTERED (UdpNdn);

TypeId
UdpNdn::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpNdn")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<UdpNdn> ()
    .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                   UintegerValue (9),
                   MakeUintegerAccessor (&UdpNdn::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("Name", "Name the node has",
                   StringValue ("kari"),
                   MakeStringAccessor (&UdpNdn::m_name),
                   MakeStringChecker())                   
    .AddTraceSource ("Rx", "A packet has been received",
                     MakeTraceSourceAccessor (&UdpNdn::m_rxTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("RxWithAddresses", "A packet has been received",
                     MakeTraceSourceAccessor (&UdpNdn::m_rxTraceWithAddresses),
                     "ns3::Packet::TwoAddressTracedCallback")
  ;
  return tid;
}

UdpNdn::UdpNdn ()
{
  NS_LOG_FUNCTION (this);
}

UdpNdn::~UdpNdn()
{
  NS_LOG_FUNCTION (this);
  m_serverSocket = 0;
  m_clientSocket = 0;
}

void
UdpNdn::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
UdpNdn::StartApplication (void)
{
  NS_LOG_FUNCTION (this);
  SetMyIp();
  std::cout << m_myAddress << std::endl;
  if (m_serverSocket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_serverSocket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      if (m_serverSocket->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
    }
  
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  m_clientSocket = Socket::CreateSocket (GetNode (), tid); //socketの生成
  #if 0   
  uint32_t pport =  269;                                                                   //アプリ層にて、dsdvパケットをリスンしたかった
  m_routingSocket = Socket::CreateSocket (GetNode (), tid); //socketの生成
  m_routingSocket->BindToNetDevice (m_device);
  m_routingSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), pport));
  m_routingSocket->SetAllowBroadcast (true);
  m_routingSocket->SetRecvCallback (MakeCallback (&UdpNdn::HandleRead2, this));
  #endif
  Ipv4Address ad ("10.0.0.3");                              // Address型に変換 

  InetSocketAddress local = InetSocketAddress (ad, m_port); //IP+port でソケットアドレス生成
  m_serverSocket->SetRecvCallback (MakeCallback (&UdpNdn::HandleRead, this)); //パケットを受信したら、こいつが呼ばれる
  m_clientSocket->SetRecvCallback (MakeCallback (&UdpNdn::HandleRead, this)); 
   
  
  //if(m_myAddress == Ipv4Address("10.0.0.1")){
    Simulator::Schedule (Seconds(0.), &UdpNdn::Send, this, local);
  //}
  Simulator::Schedule (Seconds(0.0), &UdpNdn::Print, this);
  Simulator::Schedule (Seconds(0.0), &UdpNdn::RoutingPacketSend, this);
  Simulator::Schedule (Seconds(4.0), &UdpNdn::Print, this);
  Simulator::Schedule (Seconds(6.0), &UdpNdn::Print, this);
  Simulator::Schedule (Seconds(8.0), &UdpNdn::Print, this);
  Simulator::Schedule (Seconds(10.0), &UdpNdn::Print, this);
  Simulator::Schedule (Seconds(12.0), &UdpNdn::Print, this);
  Simulator::Schedule (Seconds(14.0), &UdpNdn::Print, this);
  Simulator::Schedule (Seconds(5.0), &UdpNdn::Send, this, local);
  Simulator::Schedule (Seconds(16.0), &UdpNdn::Print, this);
}
void 
UdpNdn::Send (InetSocketAddress local)
{
  std::cout << "Send First Interest" << std::endl;
  
    NdnPacket snp ("/Osaka/weather", 0, "");
    std::stringstream sss;                         //送信するパケット用
    {
        cereal::JSONOutputArchive o_archive(sss);
        o_archive(snp);
    }  
    uint16_t packetSize = sss.str().length();
    Ptr<Packet>interestPacket = Create<Packet>((uint8_t *)sss.str().c_str(), packetSize+1);
    m_clientSocket->SendTo(interestPacket, 1, local);
}

void 
UdpNdn::HandleRead2 (Ptr<Socket> socket)
{
  std::cout << "ぶんぶん" << std::endl;
}

void 
UdpNdn::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_clientSocket != 0) 
    {
      m_clientSocket->Close ();
      m_clientSocket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
  if (m_serverSocket != 0) 
  {
    m_serverSocket->Close ();
    m_serverSocket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  }
}

/* 自分のipv4関連の情報をポインタ変数として取得*/
void
UdpNdn::SetMyIp (void)
{  
  Ptr<Node> node = this->GetNode(); //自分自身のポインタ変数(Nodeクラス)
  m_device = node->GetDevice(0); //自分自身のポインタ変数(NetDeviceクラス)
  std::cout << "私のマックアドレスは:" << m_device->GetAddress() << "です" << std::endl;
  m_device->AddLinkChangeCallback(MakeCallback(&UdpNdn::Print, this));
  m_ipv4 = node->GetObject<Ipv4> ();
  m_l3 = m_ipv4->GetObject<Ipv4L3Protocol> ();
  m_protocol = m_ipv4->GetRoutingProtocol ();
  //m_arp = m_l3->GetInterface (1)->GetArpCache (); //自分自身のポインタ変数(ArpCacheクラス)
  Ipv4InterfaceAddress iface = m_l3->GetAddress (1,0);
  if (m_myAddress == Ipv4Address ())
  {
    m_myAddress = iface.GetLocal ();
  }
  m_memberList.insert(std::make_pair(m_name, m_myAddress));         //distance-vectorにて自身の名前を広報するためにも使用
  
}

void 
UdpNdn::HandleRead (Ptr<Socket> socket) //受信処理
{
  NS_LOG_FUNCTION (this << socket);

  Ptr<Packet> packet;
  Ptr<Packet> dataPacket;
  Address from;          //送信元IPアドレス
  Address localAddress;
  contents["/Osaka/weather"] = "Sunny";
  while ((packet = socket->RecvFrom (from)))
  {
    uint8_t *buffer = new uint8_t[packet->GetSize()];
    packet->CopyData(buffer, packet->GetSize());           //ペイロードをコピ
    std::cout << buffer << std::endl;
    std::stringstream rss;
    rss << buffer;
    NdnPacket rnp ("0", 0, "0");                         //オブジェクト生成
    cereal::JSONInputArchive i_archive(rss);               //デシリアライズ 
    i_archive(rnp);                                      //オブジェクトにオーバーロード
    std::string receivedPacketName = rnp.GetName();

    if(rnp.GetTypeId() == 0)
    {
      std::cout << "Received Interest: name = " << receivedPacketName << std::endl;
      if(contents.count(receivedPacketName) != 0){
        NdnPacket snp (receivedPacketName, 1, contents[receivedPacketName]);
        std::stringstream sss;                         //送信するパケット用
        {
            cereal::JSONOutputArchive o_archive(sss);
            o_archive(snp);
        }      
        uint16_t packetSize = sss.str().length();
        Ptr<Packet> dataPacket = Create<Packet>((uint8_t *)sss.str().c_str(), packetSize+1);
        socket->SendTo(dataPacket, 0, from);      //ソースとデスティネーションひっくり返して送る
      } 
    }
    else
    {
      std::cout << "Received Data:" << rnp.GetContent() << std::endl;

    }
    
    
    packet->RemoveAllPacketTags ();
    packet->RemoveAllByteTags ();

  }
}
void
UdpNdn::Print(void)
{
  std::cout << "現在の時刻は" << Simulator::Now()  << "**************************************" << std::endl;
  Ptr<Ipv4RoutingProtocol> protocol = m_ipv4->GetRoutingProtocol ();
  Ptr<OutputStreamWrapper> osw = Create<OutputStreamWrapper> (&std::cout);
  if(m_myAddress == Ipv4Address("10.0.0.1")){
  //protocol->PrintRoutingTable(osw);
  }
  //m_arp->PrintArpCache(osw);
  //m_arp->Flush();
}
void
UdpNdn::RoutingPacketSend (void)
{
 m_periodicUpdateInterval = m_protocol->SetUp();
 
}
} // Namespace ns3
