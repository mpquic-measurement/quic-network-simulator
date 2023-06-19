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
    ./test_client -G -a 193.167.100.100 -p 4433 -o ./clog -w result -l d -n 1 -c r
elif [ "$ROLE" == "server" ]; then
    ./test_server -l d -p 4433 -s 1048576
fi
