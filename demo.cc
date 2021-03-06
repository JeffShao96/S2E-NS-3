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
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "/home/s2e/s2e/s2e.h"
#include "ctime"

//Network Topology
//
// snda -------rcv------- sndb
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{

  Time::SetResolution (Time::MS); 
  
  // No Log Infomation Allowed in this mode!

  // LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
  // LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);

  CommandLine cmd;
  cmd.Parse (argc, argv);

  NodeContainer nodes;
  nodes.Create (3); 

  std::vector<NodeContainer> nodeAdjacencyList (2);
  nodeAdjacencyList[0] = NodeContainer (nodes.Get (0), nodes.Get (2));
  nodeAdjacencyList[1] = NodeContainer (nodes.Get (1), nodes.Get (2));

  std::vector<PointToPointHelper> pointToPoint (2);
  pointToPoint[0].SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint[0].SetChannelAttribute ("SymbolicMode", BooleanValue (true));
  pointToPoint[0].SetChannelAttribute ("DelayMin", StringValue ("0s"));
  pointToPoint[0].SetChannelAttribute ("DelayMax", StringValue ("1s"));


  pointToPoint[1].SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint[1].SetChannelAttribute ("SymbolicMode", BooleanValue (true));
  pointToPoint[1].SetChannelAttribute ("DelayMin", StringValue ("0s"));
  pointToPoint[1].SetChannelAttribute ("DelayMax", StringValue ("1s"));

  std::vector<NetDeviceContainer> devices (2);
  devices[0] = pointToPoint[0].Install (nodeAdjacencyList[0]);
  devices[1] = pointToPoint[1].Install (nodeAdjacencyList[1]);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  std::vector<Ipv4InterfaceContainer> interfaces (2);
  for (uint32_t i = 0; i < 2; i++)
    {
      std::ostringstream subset;
      subset << "10.1." << i + 1 << ".0";
      address.SetBase (subset.str().c_str (), "255.255.255.0"); 
      interfaces[i] =
          address.Assign (devices[i]); 
    }

  UdpServerHelper server1 (2333);

  ApplicationContainer rcv = server1.Install (nodes.Get (2));
  rcv.Start (Seconds (1.0));
  rcv.Stop (Seconds (10.0));

  UdpClientHelper client1 (interfaces[0].GetAddress (1), 2333);
  client1.SetAttribute ("MaxPackets", UintegerValue (1));
  client1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  client1.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer snda = client1.Install (nodes.Get (0));
  snda.Start (Seconds (1.0));
  snda.Stop (Seconds (10.0));

  UdpClientHelper client2 (interfaces[1].GetAddress (1), 2333);
  client2.SetAttribute ("MaxPackets", UintegerValue (1));
  client2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  client2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer sndb = client2.Install (nodes.Get (1));
  sndb.Start (Seconds (1.0));
  sndb.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  s2e_kill_state(0,"Program Terminated");
  return 0;
}
