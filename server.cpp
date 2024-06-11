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

int main(int argc, char *argv[])
{
    struct hostent * server = gethostbyname("localhost");

    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(51717);
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);

    int ret = 0;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT(fd);
    ret = bind(fd, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
    ASSERT(ret);
    ret = listen(fd, 5);
    ASSERT(ret);
    while(true)
    {
        bool exit = false;
        socklen_t clientLenght = sizeof(clientAddr);
        int newClient = accept(fd, (struct sockaddr *) & clientAddr, &clientLenght);
        while(!exit)
        {
            char readBuffer[256] = {0};
            int readLength = read(newClient, &readBuffer, 256);
            printf("%s", &readBuffer[0]);
            if(strncmp(&readBuffer[0], "exit", 4) == 0) break;
            int writeLength = write(newClient, &readBuffer, strlen(readBuffer));
        }
        close(newClient);
    }
    close(fd);
    return 0;
}
