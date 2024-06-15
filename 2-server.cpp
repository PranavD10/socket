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
    sockaddr_in serverAddr, clientAddr;
    struct hostent * server = gethostbyname("localhost");
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port =htons(51717);
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    int ret = 0, fd = 0;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT(fd);
    ret = bind(fd, (struct sockaddr *) & serverAddr, sizeof(serverAddr));
    ASSERT(ret);
    // ret = listen(fd, 5);
    // ASSERT(ret);
    socklen_t clientAddrLen = sizeof(clientAddr);
    // ret = accept(fd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    // ASSERT(ret);
    char buffer [256] = {0};
    int len = recvfrom(fd, &buffer, 256, 0, (struct sockaddr *)& clientAddr, &clientAddrLen);
    ASSERT(len);
    printf("received %s\n", &buffer[0]);
    strcpy(&buffer[0] + strlen(buffer), ", you too");
    clientAddrLen = sizeof(clientAddr);
    len = sendto(fd, &buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    ASSERT(len);
    ret = close(fd);
    ASSERT(ret);
    return 0;
}

