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

NS_LOG_COMPONENT_DEFINE ("UdpNdnServerApplication");

NS_OBJECT_ENSURE_REGISTERED (UdpNdnServer);

TypeId
UdpNdnServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpNdnServer")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<UdpNdnServer> ()
    .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                   UintegerValue (9),
                   MakeUintegerAccessor (&UdpNdnServer::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddTraceSource ("Rx", "A packet has been received",
                     MakeTraceSourceAccessor (&UdpNdnServer::m_rxTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("RxWithAddresses", "A packet has been received",
                     MakeTraceSourceAccessor (&UdpNdnServer::m_rxTraceWithAddresses),
                     "ns3::Packet::TwoAddressTracedCallback")
  ;
  return tid;
}

UdpNdnServer::UdpNdnServer ()
{
  NS_LOG_FUNCTION (this);
}

UdpNdnServer::~UdpNdnServer()
{
  NS_LOG_FUNCTION (this);
  m_socket = 0;
  m_socket6 = 0;
}

void
UdpNdnServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
UdpNdnServer::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      if (m_socket->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
      if (addressUtils::IsMulticast (m_local))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, m_local);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local6 = Inet6SocketAddress (Ipv6Address::GetAny (), m_port);
      if (m_socket6->Bind (local6) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
      if (addressUtils::IsMulticast (local6))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket6);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, local6);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }

  m_socket->SetRecvCallback (MakeCallback (&UdpNdnServer::HandleRead, this));
  m_socket6->SetRecvCallback (MakeCallback (&UdpNdnServer::HandleRead, this));
}

void 
UdpNdnServer::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_socket != 0) 
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
  if (m_socket6 != 0) 
    {
      m_socket6->Close ();
      m_socket6->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void 
UdpNdnServer::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  Ptr<Packet> packet;
  Ptr<Packet> dataPacket;
  uint32_t dataSize;
  uint8_t *data;
  Address from;          //送信元IPアドレス
  Address localAddress;
  contents["/Osaka/weather"] = "Sunny";
  while ((packet = socket->RecvFrom (from)))
  {
    data = 0;
    dataSize = 0;
    uint8_t *buffer = new uint8_t[packet->GetSize()];
    packet->CopyData(buffer, packet->GetSize());           //ペイロードをコピー
    std::string s = std::string(buffer, buffer + packet->GetSize() - 1);
    std::cout << "Received:" << s << std::endl;
    
    packet->RemoveAllPacketTags ();
    packet->RemoveAllByteTags ();

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
      socket->SendTo(dataPacket, 0, from); //送信部分(destination addressを指定)
    } 
    //Dataパケットを返信
    //socket->SendTo (packet, 0, from); //送信部分(destination addressを指定)
    }
}

} // Namespace ns3
