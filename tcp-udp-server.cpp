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


int main(int argc, char const *argv[])
{
    struct hostent * server = gethostbyname("localhost");
    struct sockaddr_in serverAddr = {0}, clientAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(51717);
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    int tcpfd, udpfd;
    tcpfd = socket(AF_INET, SOCK_STREAM, 0);

    socklen_t clientAddrLen = sizeof(serverAddr);
    bind(tcpfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(tcpfd, 5);


    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(udpfd, (struct sockaddr *)& serverAddr, sizeof(serverAddr));

    fd_set fds;

    int maxFd = max(tcpfd, udpfd) + 1;

    bool bexit = false;
    FD_ZERO(&fds);
    while(!bexit)
    {
        FD_SET(tcpfd, &fds);
        FD_SET(udpfd, &fds);
        
        int ret = select(maxFd, &fds, NULL, NULL, NULL);
        
        if(FD_ISSET(tcpfd, &fds))
        {
            int clientFd = accept(tcpfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
            if(fork() == 0)
            {
                bool exitb = false;
                while(!exitb)
                {

                // close(tcpfd);
                char buffer[256] = {0};
                size_t len = read(clientFd, &buffer[0], sizeof(buffer));
                printf("tcp %ld : received %s\n", len, &buffer[0]);
                strcpy(&buffer[len - 1], " received");
                len = write(clientFd, &buffer[0], strlen(buffer));
                if(strncmp("exit", &buffer[0], 4) == 0)
                {
                    exitb = true;
                    close(clientFd);
                }
                }
                exit(0);
            }
            close(clientFd);
        }

        if(FD_ISSET(udpfd, &fds))
        {
            if(fork() == 0)
            {
                char buffer[256] = {0};
                socklen_t clientAddrLen = sizeof(clientAddr);
                size_t len = recvfrom(udpfd, &buffer[0], sizeof(buffer), 0, (struct sockaddr *)& clientAddr, &clientAddrLen);
                printf("udp : received %s\n", &buffer[0]);
                strcpy(&buffer[len - 1], " received");
                sendto(udpfd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
                exit(0);
            }
        }
    }

    // read();
    // write();
    // close();
    // close();

    return 0;
}
