#pragma once
#ifdef _WIN32
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
#define sleep(s) Sleep((s) * 1000)
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#define SOCKET int
#define closesocket close
#endif
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void klog(const char *fmt, ...) {
  time_t now;
  time(&now);
  struct tm *ti = localtime(&now);
  printf("[%02d-%02d-%02d %02d:%02d:%02d] ", ti->tm_year + 1900, ti->tm_mon + 1,
         ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);

  va_list arg;
  va_start(arg, fmt);
  vprintf(fmt, arg);
  va_end(arg);
}

#define KCHECK_ERRNO(expr)                                                     \
  do {                                                                         \
    if ((expr) == -1) {                                                        \
      klog("%s: %s\n", #expr, strerror(errno));                                \
      return -1;                                                               \
    }                                                                          \
  } while (false)

int init_addr(const char *ip, int port, struct sockaddr_in *addr) {
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  return 1 == inet_pton(addr->sin_family, ip, &addr->sin_addr) ? 0 : -1;
}

int setget_socket_bufsize(SOCKET s, bool snd, int size)
{
    int optname = snd ? SO_SNDBUF : SO_RCVBUF;
    KCHECK_ERRNO(setsockopt(s, SOL_SOCKET, optname, (void*)&size, sizeof(size)));

    int new_size = -1;
    socklen_t opt_len = sizeof(new_size);
    KCHECK_ERRNO(getsockopt(s, SOL_SOCKET, optname, (void*)&new_size, &opt_len));
    return new_size;
}

void init_syslib()
{
#ifdef _WIN32
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif
}
