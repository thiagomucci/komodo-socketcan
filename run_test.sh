#!/bin/bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan 2>/dev/null
sudo ip link set up vcan0

gcc -c src/komodo.c -o src/komodo.o -I.
g++ src/bridge.cpp src/komodo.o -o src/bridge -I. -ldl
g++ src/test_bridge.cpp -o tests/test_bridge
echo "Compilation completed tests ready for execution"