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
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "udp-ndn-server.h"
#include <map>
#include "ndn-packet.h"
#include "/home/nuc1/Simulation/cereal-1.3.0/include/cereal/archives/json.hpp"
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

  m_serverSocket->SetRecvCallback (MakeCallback (&UdpNdn::HandleRead, this)); //パケットを受信したら、こいつが呼ばれる
  Simulator::Schedule (Seconds(0.), &UdpNdn::Send, this);
}
void 
UdpNdn::Send (void)
{
  std::cout << "Send" << std::endl;

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  m_clientSocket = Socket::CreateSocket (GetNode (), tid); //socketの生成
  Ipv4Address ad ("10.1.1.1");                              // Address型に変換
  InetSocketAddress local = InetSocketAddress (ad, m_port); //IP+port でソケットアドレス生成

  NdnPacket snp ("/Osaka/weather", 0, "");
  std::stringstream sss;                         //送信するパケット用
  {
      cereal::JSONOutputArchive o_archive(sss);
      o_archive(snp);
  }      
  uint16_t packetSize = sss.str().length();
  Ptr<Packet>interestPacket = Create<Packet>((uint8_t *)sss.str().c_str(), packetSize);
  m_clientSocket->SendTo(interestPacket, 1, local);
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

void 
UdpNdn::HandleRead (Ptr<Socket> socket) //受信処理
{
  NS_LOG_FUNCTION (this << socket);

  Ptr<Packet> packet;
  Ptr<Packet> dataPacket;
  Address from;          //送信元IPアドレス
  Address localAddress;
  contents["/Osaka/weather"] = "Sunny";
  std::cout << "okokokokokokok" << std::endl;
  while ((packet = socket->RecvFrom (from)))
  {
    uint8_t *buffer = new uint8_t[packet->GetSize()];
    packet->CopyData(buffer, packet->GetSize());           //ペイロードをコピー
    std::stringstream rss;
    rss << buffer;                                         //buffer内容をssにロード
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
        Ptr<Packet> dataPacket = Create<Packet>((uint8_t *)sss.str().c_str(), packetSize);
        socket->SendTo(dataPacket, 0, from);      //ソースとデスティネーションひっくり返して送る
      } 
    }
    else
    {
      std::cout << "Received Data:" << rnp.GetContent()<< std::endl;

    }
    
    
    packet->RemoveAllPacketTags ();
    packet->RemoveAllByteTags ();

#if 0 
    NdnPacket np ("aaa", 1, "bbb");
    std::cout << np.GetName() << std::endl; 

    std::stringstream ss;
    {
        cereal::JSONOutputArchive o_archive(ss);
        o_archive(np);
    }
    std::cout << ss.str() << std::endl;

    NdnPacket np_i ("0", 0, "0");
    cereal::JSONInputArchive i_archive(ss);
    i_archive(np_i);

    std::cout << np_i.GetName() << std::endl;

    uint16_t packetSize = ss.str().length();
    Ptr<Packet> packeto = Create<Packet>((uint8_t *)ss.str().c_str(), packetSize);
    std::cout << from << std::endl;
    socket->SendTo(packeto, 0, from);
#endif 


#if 0

    //データがあれば
    if(contents.count(s) != 0){
      // dataSize = contents[s].size () + 1;
      // delete [] data;
      // data = new uint8_t [dataSize];

      dataSize = s.size() + 1 + contents[s].size () + 1;
      delete [] data;
      data = new uint8_t [dataSize];
      std::string payload = s + ":" + contents[s];
      std:: cout << payload << std::endl;
      memcpy (data, payload.c_str (), dataSize);
      dataPacket = Create<Packet>(data, dataSize);
      // socket->SendTo(dataPacket, 0, from); //送信部分(destination addressを指定)
    } 
    //Dataパケットを返信
    //socket->SendTo (packet, 0, from); //送信部分(destination addressを指定
  #endif
  }
}

} // Namespace ns3
