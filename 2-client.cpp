#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

using namespace std;

#define ASSERT(err)                                                   \
    do                                                                \
    {                                                                 \
        if (err < 0)                                                  \
        {                                                             \
            printf("[%s][%d] error : %d\n", __FILE__, __LINE__, err); \
            fflush(stdout);                                           \
            exit(err);                                                \
        }                                                             \
    } while (0)

#define DEBUG printf("[%d]\n", __LINE__)

int main()
{
    sockaddr_in clientAddr;
    struct hostent * server = gethostbyname("localhost");
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port =htons(51717);
    memcpy(&clientAddr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    int ret = 0, fd = 0;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT(fd);
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer [256] = {0};
    strcpy(&buffer[0], "Hello World");
    clientAddrLen = sizeof(clientAddr);
    int len = sendto(fd, &buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    ASSERT(len);
    len = recvfrom(fd, &buffer, 256, 0, (struct sockaddr *)& clientAddr, &clientAddrLen);
    printf("received %s\n", &buffer[0]);
    ASSERT(len);
    ret = close(fd);
    ASSERT(ret);
    return 0;
}

