#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int create_tun_device(char *dev_name, int flag) {
        struct ifreq ifr;
        int fd, err_code;

        // 创建 tun fd
        if ((fd = open("/dev/net/mmq", O_RDWR)) < 0) {
                printf("Tun device fd create error. (%d)\n", fd);
                return fd;
        }

        // 配置 ifreq，它是一个用于socket ioctl配置的结构
        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, dev_name);
        ifr.ifr_flags |= flag;

        // 配置 tun 设备
        if ((err_code = ioctl(fd, TUNSETIFF, &ifr)) < 0) {
                printf("Tun device ioctl configure error. (%d)\n", err_code);
                close(fd);
                return err_code;
        }

        return fd;
}

void main(int argc, char* argv[]) {


        //创建 tun 设备
        int tun_fd = create_tun_device("mmq-eth", IFF_TUN);
        if (tun_fd < 0) {
                printf("Tun device create error. (%d)\n", tun_fd);
        }
        printf("Tun device create success, start to access packet.\n");

        //不断尝试从设备中读取信息
        int ret_length = 0;
        unsigned char buf[1024];
        while (1) {
                ret_length = read(tun_fd, buf, sizeof(buf));
                if (ret_length < 0) {
                        break;
                }
                //分析报文
                unsigned char src_ip[4];
                unsigned char dst_ip[4];
                memcpy(src_ip, &buf[16], 4);
                memcpy(dst_ip, &buf[20], 4);
                printf("ICMP receive : %d.%d.%d.%d -> %d.%d.%d.%d (%d)\n", dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3], src_ip[0], src_ip[1], src_ip[2], src_ip[3], ret_length);

                //返回icmp应答报文
                memcpy(&buf[16], dst_ip, 4);
                memcpy(&buf[20], src_ip, 4);
                buf[24] = 0;
                ret_length = write(tun_fd, buf, ret_length);
                printf("ICMP send : %d.%d.%d.%d -> %d.%d.%d.%d (%d)\n", src_ip[0], src_ip[1], src_ip[2], src_ip[3], dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3], ret_length);

        }

        //关闭设备
        close(tun_fd);
}
