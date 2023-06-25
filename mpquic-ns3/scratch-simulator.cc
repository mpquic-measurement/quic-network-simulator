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
#include "ns3/quic-server.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/quic-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/random-variable-stream.h"
#include <iostream>
#include "ns3/flow-monitor-module.h"
#include "ns3/gnuplot.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ScratchSimulator");

int
main(int argc, char* argv[])
{
    NS_LOG_UNCOND("Scratch Simulator");

    Ptr<Node> server = CreateObject<Node>();
    Ptr<Node> client = CreateObject<Node>();


    NodeContainer c2s = NodeContainer(client, server);

    QuicHelper quic_stack;
    quic_stack.InstallQuic(server);
    quic_stack.InstallQuic(client);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("10ms"));

    NetDeviceContainer net_c2s = p2p.Install(c2s);

    QuicClient client_c;
    client_c.SetNode(client);


    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer ip_c2s = ipv4.Assign (net_c2s);

    std::cout << ip_c2s.GetAddress(0) << std::endl;
    std::cout << ip_c2s.GetAddress(1) << std::endl;



    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
