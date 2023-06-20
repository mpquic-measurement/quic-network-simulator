#ifndef MPQUIC_NETWORK_SIMULATOR_HELPER_H
#define MPQUIC_NETWORK_SIMULATOR_HELPER_H

#include "ns3/node.h"

using namespace ns3;

class MPQuicNetworkSimulatorHelper {
public:
  MPQuicNetworkSimulatorHelper();
  void Run(Time);
  Ptr<Node> GetLeftNode() const;
  Ptr<Node> GetRightNode() const;
  
  Ptr<Node> GetServerNode() const;
  Ptr<Node> GetClientNode0() const;
  Ptr<Node> GetClientNode1() const;

private:
  void RunSynchronizer() const;
  Ptr<Node> left_node_, right_node_;

  Ptr<Node> server_node_;
  Ptr<Node> client_node_0;
  Ptr<Node> client_node_1;
};

#endif /* QUIC_NETWORK_SIMULATOR_HELPER_H */
