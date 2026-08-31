#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int open_socketcan(const char *ifname) {
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
    int sock = open_socketcan("vcan0");
    if (sock < 0)
    {
        std::cerr << "Failed to open SocketCAN: " << std::strerror(errno) << "\n";
        return 1;
    }

    struct can_frame frame{};
    frame.can_id = 0x123;
    frame.can_dlc = 2;
    frame.data[0] = 0xAA;
    frame.data[1] = 0xBB;

    ssize_t bytes_written = write(sock, &frame, sizeof(frame));
    if (bytes_written != sizeof(frame))
    {
    std::cerr << "error writing to SocketCAN: " << std::strerror(errno) << "\n";
    close(sock);
    return 1;
    }
    std::cout << "test frame sent" << "\n";

    close(sock);
    return 0;
}