/* server.c */
#include "utils.h"

int main()
{
    init_syslib();

    struct sockaddr_in addr = { 0 };
    KCHECK_ERRNO(init_addr("0.0.0.0", 8888, &addr));

    SOCKET sockfd;
    KCHECK_ERRNO(sockfd = socket(AF_INET, SOCK_STREAM, 0));
    KCHECK_ERRNO(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)));
    KCHECK_ERRNO(listen(sockfd, 10));

    int set_size = 2048;
    int get_size = setget_socket_bufsize(sockfd, false, set_size);
    if (get_size != set_size) {
        klog("set recv buffer size failed. set: %d, get: %d\n", set_size, get_size);
        // return -1;
    }

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int client_sockfd = 0;
    while (1) {
        klog("server: accepting ...\n");

        KCHECK_ERRNO(client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr,
                         &client_addrlen));

        klog("accept new connection\n");

        char buf[2048] = { 0 };
        int total = 0;
        int n = 0;
        while ((n = recv(client_sockfd, buf, sizeof(buf), 0)) > 0) {
            total += n;
            klog("recv: %d\n", n);
            sleep(6);
        }
        klog("total recv: %d\n", total);

        klog("server: closing socket ...\n");
        closesocket(client_sockfd);
        klog("server: closed socket!\n");
    }

    return 0;
}
