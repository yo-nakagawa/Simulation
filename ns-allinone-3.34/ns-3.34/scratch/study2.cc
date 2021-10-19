/* -*-  Mode: C++; nodes-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
お試し
adhoc + constantplace + + UDP(NDN)
 */
#include "ns3/core-module.h"
#include "ns3/gnuplot.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/mobility-model.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/netanim-module.h"
#include "ns3/aodv-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Wifi-Adhoc");

class Experiment
{
public:
  Experiment ();
  Experiment (std::string name);
  Gnuplot2dDataset Run ();
private:
  // network
  /// nodes used in this simulation
  NodeContainer nodes;
  /// devices used in this simulation
  NetDeviceContainer devices;
  /// interfaces used in this simulation
  Ipv4InterfaceContainer interfaces;
  /// pointer to node
  Ptr<Node> leaderNode;
  /// port number
  int portNum;

private:
  void ReceivePacket (Ptr<Socket> socket);
  void SetPosition (Ptr<Node> node, Vector position);
  Vector GetPosition (Ptr<Node> node);
  void AdvancePosition (Ptr<Node> node);
  Ptr<Socket> SetupPacketReceive (Ptr<Node> node);
  
  /// Create the nodes
  void CreateNodes ();
  /// Create the devices
  void CreateDevices ();
  /// Create the network
  void InstallInternetStack ();
  /// Create the simulation applications
  void InstallApplications ();

  uint32_t m_bytesTotal;
  Gnuplot2dDataset m_output;
};

Experiment::Experiment ()
{
}

Experiment::Experiment (std::string name)
  : m_output (name)
{
  m_output.SetStyle (Gnuplot2dDataset::LINES);
}

void
Experiment::SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector
Experiment::GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void
Experiment::AdvancePosition (Ptr<Node> node)
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / 1000000);
  m_bytesTotal = 0;
  m_output.Add (pos.x, mbs);
  pos.x += 1.0;
  if (pos.x >= 210.0)
    {
      return;
    }
  SetPosition (node, pos);
  Simulator::Schedule (Seconds (1.0), &Experiment::AdvancePosition, this, node);
}

void
Experiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  while ((packet = socket->Recv ()))
    {
      m_bytesTotal += packet->GetSize ();
    }
}

Ptr<Socket>
Experiment::SetupPacketReceive (Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::PacketSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);
  sink->Bind ();
  sink->SetRecvCallback (MakeCallback (&Experiment::ReceivePacket, this));
  return sink;
}

void
Experiment::CreateNodes ()
{
//ノードの作成
  nodes.Create (2);
  leaderNode = nodes.Get (0);
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
}

void
Experiment::CreateDevices ()
{
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac"); //アドホックモードを指定
  YansWifiPhyHelper wifiPhy;
  //YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel",
                                  "MaxRange", DoubleValue (13.0)); //wifiの距離
  wifiPhy.SetChannel (wifiChannel.Create ()); //チャネルの設定 (Createメソッドで新チャネルが返ってくる)
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", UintegerValue (0));
  devices = wifi.Install (wifiPhy, wifiMac, nodes); //設定をノードにインストール
  wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);//パケットキャプチャ
  wifiPhy.EnablePcapAll ("pcap/study2"); //パケットキャプチャ
}

void
Experiment::InstallInternetStack ()
{
  AodvHelper aodv;
  // you can configure AODV attributes here using aodv.Set(name, value)
  InternetStackHelper stack;
  stack.SetRoutingHelper (aodv); // has effect on the next Install ()
  stack.Install (nodes);
  Ipv4AddressHelper address; //IPv4の割当
  address.SetBase ("10.0.0.0", "255.0.0.0");
  interfaces = address.Assign (devices);
}

void
Experiment::InstallApplications ()
{
  /****ping****
  V4PingHelper ping ("10.0.0.2");//ここから、変更
  ping.SetAttribute ("Verbose", BooleanValue (true));
  ApplicationContainer apps = ping.Install (leaderNode);
  apps.Start (Seconds (3.0));
  apps.Stop (Seconds (8.0));  
  */

    /* Install UDP Receiver on the access point */
  portNum = 50000;
  PacketSinkHelper sinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), portNum));
  ApplicationContainer sinkApp = sinkHelper.Install (leaderNode);
  sinkApp.Start (Seconds (3.0));
  sinkApp.Stop (Seconds (8.0));


  /* Install UDP Transmitter on the station */
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (1.);
  UdpEchoClientHelper client (interfaces.GetAddress (0), portNum);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  int nNodes = nodes.GetN();
  ApplicationContainer apps;
  for (int i = 1; i < nNodes ; ++i)
  {
    
    apps.Add (client.Install (nodes.Get(i)));
  }
  client.SetFill (apps.Get (0), "Hello World");
  apps.Start (Seconds (3.0));
  apps.Stop (Seconds (8.0));
  

}

Gnuplot2dDataset
Experiment::Run ()
{
  m_bytesTotal = 0;
 
  std::string animFile = "xml/study2-animation.xml";
  
  CreateNodes();
  CreateDevices();
  InstallInternetStack();
  InstallApplications();

  Simulator::Stop(Seconds (10.0));
  AnimationInterface anim (animFile);
  Simulator::Run ();

  Simulator::Destroy ();

  return m_output;
}

int main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Experiment experiment;
  experiment = Experiment();
  experiment.Run ();

  return 0;
}