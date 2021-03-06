/* client.c */
#include "utils.h"

int main()
{
    init_syslib();

    struct sockaddr_in addr = { 0 };
    KCHECK_ERRNO(init_addr("192.168.50.102", 8888, &addr));

    SOCKET sockfd;
    KCHECK_ERRNO(sockfd = socket(AF_INET, SOCK_STREAM, 0));

    int set_size = 64256 * 3;
    int get_size = setget_socket_bufsize(sockfd, true, set_size);
    if (get_size != set_size) {
        klog("set send buffer size failed. set: %d, get: %d\n", set_size, get_size);
        // return -1;
    }

#if 1
    struct linger l_opt;
    l_opt.l_onoff = 1;
    l_opt.l_linger = 3;
    KCHECK_ERRNO(
        setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (void*)&l_opt, sizeof(l_opt)));
#endif

    KCHECK_ERRNO(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)));

    klog("client: connected\n");

    char buf[64256 * 3] = { 0 };
    for (int i = 0; i < 1; ++i) {
        int ret = send(sockfd, buf, sizeof(buf), 0);
        klog("send: %d => %d\n", i, ret);
        if (ret == -1) {
            klog("client: send");
            return 1;
        }
    }

    klog("client: closing socket ...\n");
    int ret = closesocket(sockfd);
    int en = errno;
    klog("client: closesocket return %d(%d)\n", ret, en);
    klog("client: closed socket!\n");

    return 0;
}
