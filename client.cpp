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
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0}, client = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(51717);
    struct hostent * server = gethostbyname("localhost");
    bcopy((char *)server->h_addr, (char *)&addr.sin_addr.s_addr, server->h_length);
    connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    socklen_t clientLen;
    char buffer[] = "Hello";
    char newBuffer[256] = {0};
    int writeLength = write(fd, buffer, strlen(buffer));
    usleep(1000);
    int readLength = read(fd, newBuffer, 256);
    for(int i = 0; i < readLength; i++) printf("%c", newBuffer[i]); printf("\n");
    close(fd);
    return 0;
}
