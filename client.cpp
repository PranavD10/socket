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

int main(int argc, char * argv[])
{
    struct hostent * server = gethostbyname("localhost");

    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(51717);
    memcpy(&clientAddr.sin_addr.s_addr, server->h_addr, server->h_length);

    int ret = 0;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT(fd);
    ret = connect(fd, (struct sockaddr *) & clientAddr, sizeof(clientAddr));
    ASSERT(ret);
    socklen_t clientLenght = sizeof(clientAddr);
    bool exit = false;
    while(!exit)
    {
        char readBuffer[256] = {0};
        char writeBuffer[256] = {0};
        fgets(writeBuffer, 256, stdin);
        int writeLength = write(fd, &writeBuffer, strlen(writeBuffer));
        int readLength = read(fd, &readBuffer, 256);
        printf("%s", &readBuffer[0]);
    }
    close(fd);
    return 0;
}
