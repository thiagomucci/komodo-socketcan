#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

extern "C" 
{
    #include "komodo.h"
}

int open_socketcan(const char *ifname)
{
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) return -1;

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) 
    {
        close(sock);
        return -1;
    }

    struct sockaddr_can addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sock);
        return -1;
    }

    return sock;
}

int
main()
{
    std::cout << "initializating komodo hardware" << "\n";

    Komodo km = km_open(0);
    if (km <= 0)
    {
        std::cerr << "error: Unable to open Komodo device (code: " << km << ")\n";
    } else {
        int bitrate = km_can_bitrate(km, KM_CAN_CH_A, 500000);
        std::cout << "bitrate configured for Channel A: " << bitrate << " bps\n";
        if (km_enable(km) < 0)
        {
        std::cerr << "Error enabling Komodo CAN channel.\n";
        }
    }

    int sock = open_socketcan("vcan0");
    if (sock < 0)
    {
        std::cerr << "failed to open SocketCAN: " << std::strerror(errno) << "\n";
        return 1;
    }

    struct can_frame frame;
    std::cout << "SocketCAN initialized on vcan0, connecting to Komodo hardware...\n";
    while (true)
    {
        ssize_t nbytes = read(sock, &frame, sizeof(struct can_frame));

        if (nbytes < 0)
        {
            std::cerr << "error reading from SocketCAN" << std::strerror(errno) << "\n";
            break;
        }

        if (nbytes < (ssize_t)sizeof(struct can_frame))
        {
            std::cerr << "warning: incomplete CAN frame received" << "\n";
            continue;
        }

        std::cout << "frame ID: 0x" << std::hex << frame.can_id
         << " | DLC: " << std::dec << (int)frame.can_dlc << " | dados: ";

        for (int i = 0; i < frame.can_dlc; ++i)
        {
            std::cout << std::hex << (int)frame.data[i] << " ";
        }
        std::cout << "\n";
    }
    close(sock);
    if (km > 0)
    {
        std::cout << "Closing Komodo hardware connection...\n";
        km_close(km);
    }
    return 0;
}