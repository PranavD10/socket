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
    
    struct sockaddr_in addr = {0}, client = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(51717);
    bcopy((char *)server->h_addr, (char *)&addr.sin_addr.s_addr, server->h_length);

    char buffer[256] = {0};
    char newBuffer[] = "World";
    socklen_t clientLen;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(fd, 5);
    int newfd = accept(fd, (struct sockaddr *)&client, &clientLen);
    int readLength = read(newfd, buffer, 256);
    for(int i = 0; i < readLength; i++) printf("%c", buffer[i]); printf("\n");
    usleep(1000);
    int writeLength = write(newfd, newBuffer, strlen(newBuffer));
    close(newfd);
    close(fd);
    return 0;
}
