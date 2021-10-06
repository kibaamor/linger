/* client.c */
#include "utils.h"

int main()
{
    init_syslib();

    struct sockaddr_in addr = { 0 };
    KCHECK_ERRNO(init_addr("127.0.0.1", 8888, &addr));

    int sockfd = 0;
    KCHECK_ERRNO(sockfd = socket(AF_INET, SOCK_STREAM, 0));

    int set_size = 10240;
    int get_size = setget_socket_bufsize(sockfd, true, set_size);
    if (get_size != set_size) {
        klog("set send buffer size failed. set: %d, get: %d\n", set_size, get_size);
        // return -1;
    }

#if 1
    struct linger l_opt;
    l_opt.l_onoff = 1;
    l_opt.l_linger = 0;
    KCHECK_ERRNO(
        setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (void*)&l_opt, sizeof(l_opt)));
#endif

    KCHECK_ERRNO(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)));

    klog("client: connected\n");

    char buf[10240] = { 0 };
    for (int i = 0; i <= 3278; ++i) {
        int ret = send(sockfd, buf, sizeof(buf), 0);
        klog("send: %d => %d\n", i, ret);
        if (ret == -1) {
            klog("client: send");
            return 1;
        }
    }

    klog("client: closing socket ...\n");
    closesocket(sockfd);
    klog("client: closed socket!\n");

    return 0;
}
