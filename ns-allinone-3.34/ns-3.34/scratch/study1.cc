/* -*-  Mode: C++; nodes-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
お試し
adhoc + random walk + tcp通信 
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
#include "ns3/v4ping-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet-sink-helper.h"

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
  nodes.Create (5);
  leaderNode = nodes.Get (0);
  MobilityHelper mobility;
  // mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
  //                                "X", StringValue ("50.0"),
  //                                "Y", StringValue ("50.0"),
  //                                "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=50]"));
  // mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
  //                                "MinX", DoubleValue (0.0),
  //                                "MinY", DoubleValue (0.0),
  //                                "DeltaX", DoubleValue (5.0),
  //                                "DeltaY", DoubleValue (10.0),
  //                                "GridWidth", UintegerValue (20),
  //                                "LayoutType", StringValue ("RowFirst"));
  // mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
  //                            "Mode", StringValue ("Time"),
  //                            "Time", StringValue ("2s"),
  //                            "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=3.0]"),
  //                            "Bounds", StringValue ("0|200|0|200"));
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (25.0),
                                 "MinY", DoubleValue (25.0),
                                 "DeltaX", DoubleValue (10.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (2),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                               "Time", StringValue ("2s"),
                             "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=3.0]"),
                             "Bounds", RectangleValue (Rectangle (-100, 100, -100, 100)));
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
  wifiPhy.EnablePcapAll ("study1"); //パケットキャプチャ
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
  address.SetBase ("10.0.0.0", "255.255.255.0");
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

    /* Install TCP Receiver on the access point */
  portNum = 50000;
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), portNum));
  ApplicationContainer sinkApp = sinkHelper.Install (leaderNode);
  sinkApp.Start (Seconds (3.0));
  sinkApp.Stop (Seconds (8.0));


  /* Install TCP/UDP Transmitter on the station */
  OnOffHelper server ("ns3::TcpSocketFactory", Address ());
  server.SetConstantRate(DataRate(5000)); //bps
  // server.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  // server.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  ApplicationContainer serverApps;
  int nNodes = nodes.GetN();
  for (int i = 1; i < nNodes ; ++i)
  {
    AddressValue remoteAddress (InetSocketAddress (interfaces.GetAddress (0), portNum));
    server.SetAttribute ("Remote", remoteAddress);
    serverApps.Add (server.Install (nodes.Get(i)));
  }
  serverApps.Start (Seconds (3.0));
  serverApps.Stop (Seconds (8.0));
  

}

Gnuplot2dDataset
Experiment::Run ()
{
  m_bytesTotal = 0;
 
  std::string animFile = "xml/study-animation.xml";
  
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
  // NS_LOG_DEBUG ("ideal");
  // experiment = Experiment ("ideal");
  // wifi.SetRemoteStationManager ("ns3::IdealWifiManager");  

  experiment = Experiment();
  experiment.Run ();

  return 0;
}