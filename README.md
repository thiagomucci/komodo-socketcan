# socketcan-komodo

A lightweight, low-level Linux bridge daemon in C++ connecting Total Phase Komodo CAN interfaces to Linux **SocketCAN**.

This tool allows hardware utilities, network analyzers, and ECU software (such as **SavvyCAN**, **candump**, **Wireshark**, or **ROS**) to interact directly with Komodo CAN channels as native Linux network interfaces (`canX` / `vcanX`).

## What is socketcan-komodo?

Total Phase Komodo CAN devices operate via proprietary USB vendor commands. Linux tools naturally communicate through the Kernel's native **SocketCAN** subsystem. 

`socketcan-komodo` acts as a high-performance bridge daemon:
1. Captures physical CAN frames from the Komodo USB interface.
2. Translates them into Linux `struct can_frame` packets.
3. Injects them directly into the Linux Kernel network layer.

## Features
- **Native SocketCAN Integration:** Seamless support for standard Linux CAN utilities.
- **Low Overhead:** Written in modern C++ (C++17) with direct POSIX system calls (`socket`, `bind`, `ioctl`).
- **Zero Heavy Dependencies:** Requires only standard C++ runtime and Linux Kernel headers.
- **Isolated Testing Suite:** Includes modular unit tests for local verification without physical hardware.

## System Requirements
- **Operating System:** Linux (Kernel 2.6.25 or higher with SocketCAN enabled)
- **Compiler:** `g++` with C++17 support
- **Build System:** GNU `make`
- **Hardware:** Total Phase Komodo CAN Interface (or `vcan` virtual interface for testing)

## Building and Installing
### 1. Clone the repository
```bash
git clone [https://github.com/thmuccx/komodo-socketcan.git](https://github.com/thmuccx/komodo-socketcan.git)
cd komodo-socketcan
```

### 2. Build binaries using Makefile
```bash
make
```

### To clean previous build artifacts:
```bash
make clean
```

## Running Tests
Before connecting physical hardware, verify local SocketCAN bindings using the virtual CAN driver (vcan):
```bash
setup virtual CAN interface
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

### Monitor incoming traffic in Terminal 1
```bash
candump vcan0
```

### Run local test suite in Terminal 2
```bash
./test_bridge
```

## License
This project is licensed under the zlib License — feel free to use, modify, and distribute for personal and commercial applications.
