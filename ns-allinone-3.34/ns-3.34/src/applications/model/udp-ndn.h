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

#ifndef UDP_NDN_SERVER_H
#define UDP_NDN_SERVER_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/traced-callback.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4.h" 
#include "ns3/ipv4-address.h"
#include "ns3/arp-cache.h"
#include "ns3/ipv4-l3-protocol.h"

namespace ns3 {

class Socket;
class Packet;

/**
 * \ingroup applications 
 * \defgroup udpndn UdpNdn
 */

/**
 * \ingroup udpecho
 * \brief A Udp Ndn server
 *
 * Every packet received is sent back.
 */
class UdpNdn : public Application 
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  UdpNdn ();
  virtual ~UdpNdn ();

protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  /**
   * \brief Handle a packet reception.
   *
   * This function is called by lower layers.
   *
   * \param socket the socket the packet was received to.
   */
  void Send(InetSocketAddress local);
  void HandleRead (Ptr<Socket> socket);
  void HandleRead2 (Ptr<Socket> socket);
  void SetMyIp (void);
  void Print (void);
  void RoutingPacketSend (void);

  uint16_t m_port; //!< Port on which we listen for incoming packets.
  std::string m_name; //自分のノード名
  std::map <std::string, Ipv4Address> m_memberList; //シナリオから与えた・・・☓、dsdvを通して知った・・・o {ノード名、そのノードのIPアドレスのリスト}
  Ptr<Socket> m_serverSocket; //!< IPv4 Socket
  Ptr<Socket> m_clientSocket;
  Ptr<Socket> m_routingSocket;
  Address m_local; //!< local multicast address
  Ipv4Address m_myAddress;
  Ptr<Ipv4> m_ipv4;
  Ptr<Node> m_node;
  Ptr<NetDevice> m_device;
  Ptr<Ipv4L3Protocol> m_l3;
  Ptr<Ipv4RoutingProtocol> m_protocol;
  Time m_periodicUpdateInterval;
  Timer m_periodicUpdateTimer;
  //Ptr<ArpCache> m_arp;

  /// Callbacks for tracing the packet Rx events
  TracedCallback<Ptr<const Packet> > m_rxTrace;

  /// Callbacks for tracing the packet Rx events, includes source and destination addresses
  TracedCallback<Ptr<const Packet>, const Address &, const Address &> m_rxTraceWithAddresses;
  /// 保持するコンテンツ
  std::map <std::string, std::string> contents;
};

} // namespace ns3

#endif /* UDP_ECHO_SERVER_H */

