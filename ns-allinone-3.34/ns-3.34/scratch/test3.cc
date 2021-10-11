/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
アニメーション確認
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3; //ns3を使用しますよ〜って宣言

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  std::string animFile = "xml/example-animation.xml";
  Time::SetResolution (Time::NS); //時間の設定
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);//ログの設定
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;  //ノードを定義
  nodes.Create (2);

  PointToPointHelper pointToPoint; //p2pリンクの設定
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps")); //レート設定
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));//p2pリンクを作成する際の間隔

  NetDeviceContainer devices; //上記の設定をノードに適用
  devices = pointToPoint.Install (nodes);
  //プロトコルスタックを乗せていく
  InternetStackHelper stack; //インターネットの設定
  stack.Install (nodes);   //ノードに適用

  Ipv4AddressHelper address;//IPアドレスの適用
  address.SetBase ("10.1.1.0", "255.255.255.0"); //10.1.1.0からスタート(ノード0は10.1.1.1, ノード1は10.1.1.2)

  Ipv4InterfaceContainer interfaces = address.Assign (devices); //ノード間に適用
  
  //アプリケーションレベルの設定
  UdpEchoServerHelper echoServer (9);  //UDPのecho serverアプリ(引数はポート番号)

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1)); //Nodes1にサーバを適用
  serverApps.Start (Seconds (1.0));//スタートする時間
  serverApps.Stop (Seconds (10.0));//終了する時間

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9); //1のIPアドレスとポート番号を教える
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1)); //シミュレーションで送信可能なパケットの最大数
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0))); //パケットの送信間隔
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));//ペイロードのサイズ

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));//Nodes0にクライアントを適用
  clientApps.Start (Seconds (2.0));//スタートする時間
  clientApps.Stop (Seconds (10.0));//終了する時間
  AnimationInterface anim (animFile);
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
