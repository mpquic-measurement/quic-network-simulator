#ifndef QUIC_NETWORK_SIMULATOR_HELPER_H
#define QUIC_NETWORK_SIMULATOR_HELPER_H

#include "ns3/node.h"

using namespace ns3;

class QuicNetworkSimulatorHelper {
public:
  QuicNetworkSimulatorHelper();
  void Run(Time);
  Ptr<Node> GetClientNode() const;
  Ptr<Node> GetServerNode() const;
  void InstallNetDevice(Ptr<Node> node, std::string deviceName, Mac48AddressValue macAddress, Ipv4InterfaceAddress ipv4Address, Ipv6InterfaceAddress ipv6Address);
  Mac48Address GetMac48Address(std::string iface);
  void MessageIpv6Routing(Ptr<Node> local, Ptr<Node> peer);
  Ipv4Address GetServerAddress();
  Ipv4Address GetClientAddress();

  void Install();

  Ipv4InterfaceAddress client_address;
  Ipv4InterfaceAddress server_address;
  

  std::string Role;

private:
  void RunSynchronizer() const;
  Ptr<Node> client_node_, server_node_;
};

#endif /* QUIC_NETWORK_SIMULATOR_HELPER_H */
