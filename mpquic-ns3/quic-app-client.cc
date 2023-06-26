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

// Network topology
//
//       n0 ----------- n1
//            40 Gbps
//            0.01 ms

// This programs illustrates how QUIC pacing can be used and how user can set
// pacing rate. The program gives information about each flow like transmitted
// and received bytes (packets) and throughput of that flow. Currently, it is
// using QUIC NewReno-like but in future after having congestion control
// algorithms which can change pacing rate can be used.

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/fd-net-device-helper.h"
#include "ns3/fd-net-device-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/point-to-point-module.h"
#include "ns3/quic-client-server-helper.h"
#include "ns3/quic-module.h"
#include <fstream>
#include <string>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("QuicPacingExample");

int main(int argc, char *argv[]) {
  uint32_t QUICFlows = 1;
  bool isPacingEnabled = false;
  std::string pacingRate = "1Mbps";

  Config::SetDefault("ns3::TcpSocketState::EnablePacing",
                     BooleanValue(isPacingEnabled));

  string deviceName("eth0");
  string remoteAddress("193.167.100.100");
  string localGateway("193.167.0.1");

  Ipv4Mask localMask("255.255.255.0");

  FdNetDeviceHelper *helper = nullptr;
  EmuFdNetDeviceHelper *raw = new EmuFdNetDeviceHelper;
  raw->SetDeviceName(deviceName);
  helper = raw;

  NS_LOG_INFO("Create client node.");
  Ptr<Node> clientNode = CreateObject<Node>();

  NS_LOG_INFO("Create channels.");

  NetDeviceContainer devices = helper->Install(clientNode);
  Ptr<NetDevice> device = devices.Get(0);
  device->SetAttribute("Address", Mac48AddressValue(Mac48Address::Allocate()));

  NS_LOG_INFO("Install QUIC Stack");
  QuicHelper stack;
  stack.InstallQuic(clientNode);

  NS_LOG_INFO("Create IPv4 Interface");
  Ptr<Ipv4> ipv4 = clientNode->GetObject<Ipv4>();
  uint32_t interface = ipv4->AddInterface(device);
  Ipv4InterfaceAddress address =
      Ipv4InterfaceAddress("193.167.0.100", localMask);
  ipv4->AddAddress(interface, address);
  ipv4->SetMetric(interface, 1);
  ipv4->SetUp(interface);

  NS_LOG_INFO("Setup Static Routing");
  Ipv4Address gateway(localGateway.c_str());
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> staticRouting =
      ipv4RoutingHelper.GetStaticRouting(ipv4);
  staticRouting->SetDefaultRoute(gateway, interface);

  NS_LOG_INFO("Create Applications.");
  Ipv4Address serverAddress;
  serverAddress.Set(remoteAddress.c_str());

  QuicClientHelper quicClient(serverAddress, 2048);
  ApplicationContainer clientApps;
  clientApps.Add(quicClient.Install(clientNode));

  clientApps.Start(Seconds(2.0));
  clientApps.Stop(Seconds(10));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  helper->EnablePcapAll("/logs/quic", true);
  helper->EnableAsciiAll("/logs/quic.tr");

  NS_LOG_INFO("Run Simulation.");
  Simulator::Stop(Seconds(10));
  Simulator::Run();

  monitor->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier =
      DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i =
           stats.begin();
       i != stats.end(); ++i) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
    std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> "
              << t.destinationAddress << ")\n";
    std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
    std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
    std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000
              << " Mbps\n";
    std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
    std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
    std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000
              << " Mbps\n";
  }

  Simulator::Destroy();
  NS_LOG_INFO("Done.");
}
