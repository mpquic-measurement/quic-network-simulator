#!/bin/bash

# Set up the routing needed for the simulation
/setup.sh

# The following variables are available for use:
# - ROLE contains the role of this execution context, client or server
# - SERVER_PARAMS contains user-supplied command line parameters
# - CLIENT_PARAMS contains user-supplied command line parameters

if [ "$ROLE" == "client" ]; then
    # Wait for the simulator to start up.
    /wait-for-it.sh sim:57832 -s -t 30
    echo "mpquic client"
    /ns3/build/scratch/quic-client/ns3.38-quic-client-default
elif [ "$ROLE" == "server" ]; then
    /wait-for-it.sh sim:57832 -s -t 30
    echo "server"
    /ns3/build/scratch/quic-server/ns3.38-quic-server-default
fi
