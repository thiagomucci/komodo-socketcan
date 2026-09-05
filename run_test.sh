#!/bin/bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan 2>/dev/null
sudo ip link set up vcan0

g++ bridge.cpp -o bridge
g++ test_bridge.cpp -o test_bridge

echo "Compilation completed tests ready for execution"