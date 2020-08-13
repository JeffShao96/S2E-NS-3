/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "/home/s2e/s2e/s2e.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  // CommandLine cmd;
  // cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::US);
  // LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
  // LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
  // LogComponentEnableAll (LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  // pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));



  pointToPoint.SetChannelAttribute ("PerPacketSymbolicDelay", BooleanValue (true));
  pointToPoint.SetChannelAttribute ("DelayMin", StringValue ("0ms"));
  pointToPoint.SetChannelAttribute ("DelayMax", StringValue ("15s"));





  // s2e_printf("\n【【【NetDeviceContainer】】】\n");
  // std::cout<<"\n【【【NetDeviceContainer】】】\n";
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // UdpEchoServerHelper echoServer (9);
  UdpServerHelper server (2333);

  // ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  // serverApps.Start (Seconds (1.0));
  // serverApps.Stop (Seconds (10.0));

  ApplicationContainer sapps = server.Install (nodes.Get (1));
  sapps.Start (Seconds (1.0));
  sapps.Stop (Seconds (10.0));


  UdpClientHelper client (interfaces.GetAddress (1), 2333);
  client.SetAttribute ("MaxPackets", UintegerValue (2));
  client.SetAttribute ("Interval", TimeValue (Seconds(1.0)));
  client.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer capps = client.Install (nodes.Get (0));
  capps.Start (Seconds (1.0));
  capps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  s2e_kill_state(0, "program terminated");
  return 0;
}
