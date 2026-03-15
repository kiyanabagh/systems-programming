#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd;
    int port;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buf[1024];

    if (argc != 2) {
        fprintf(stderr, "usage: ./minor4svr <port>\n");
        exit(1);
    }

    port = atoi(argv[1]);
    if (port <= 0) {
        fprintf(stderr, "invalid port\n");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    srand(12345);

    printf("[server]: ready to accept data...\n");
    fflush(stdout);

    clilen = sizeof(cliaddr);

    while (1) {
        ssize_t n;

        n = recvfrom(sockfd, buf, sizeof(buf) - 1, 0,
                     (struct sockaddr *)&cliaddr, &clilen);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        buf[n] = '\0';
        if (rand() % 10 < 3) {
            printf("[server]: dropped packet\n");
            fflush(stdout);
            continue;
        }

        printf("[client]: %s\n", buf);
        fflush(stdout);

        {
            const char *pong = "PONG";
            if (sendto(sockfd, pong, strlen(pong), 0,
                       (struct sockaddr *)&cliaddr, clilen) < 0) {
                perror("sendto");
            }
        }
    }

    close(sockfd);
    return 0;
}
