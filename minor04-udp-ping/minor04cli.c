#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int sockfd = -1;
    char *hostname;
    char *port_str;
    struct addrinfo hints, *res = NULL, *p;
    struct addrinfo *chosen = NULL;
    struct timeval tv;
    int sent = 0, received = 0;
    double min_rtt = 0.0, max_rtt = 0.0, sum_rtt = 0.0;

    if (argc != 3) {
        fprintf(stderr, "usage : ./minor4cli <hostname> <port>\n");
        exit(1);
    }

    hostname = argv[1];
    port_str = argv[2];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    {
        int ret = getaddrinfo(hostname, port_str, &hints, &res);
        if (ret != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
            exit(1);
        }
    }

    for (p = res; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET6) {
            sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (sockfd >= 0) {
                chosen = p;
                break;
            }
        }
    }

    if (sockfd < 0 || chosen == NULL) {
        for (p = res; p != NULL; p = p->ai_next) {
            if (p->ai_family == AF_INET) {
                sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
                if (sockfd >= 0) {
                    chosen = p;
                    break;
                }
            }
        }
    }

    if (sockfd < 0 || chosen == NULL) {
        fprintf(stderr, "could not create socket\n");
        if (res) freeaddrinfo(res);
        exit(1);
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                   &tv, sizeof(tv)) < 0) {
        perror("setsockopt");
    }

    for (int i = 1; i <= 10; i++) {
        char msg[128];
        char buf[1024];
        ssize_t n;
        struct timeval start, end;

        snprintf(msg, sizeof(msg), "PING %d", i);

        if (gettimeofday(&start, NULL) < 0) {
            perror("gettimeofday");
        }

        n = sendto(sockfd, msg, strlen(msg), 0,
                   chosen->ai_addr, chosen->ai_addrlen);
        if (n < 0) {
            perror("sendto");
            printf("%d: Sent... Timed Out\n", i);
            continue;
        }

        sent++;

        n = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, NULL, NULL);
        if (n < 0) {
            printf("%d: Sent... Timed Out\n", i);
            continue;
        }

        buf[n] = '\0';

        if (gettimeofday(&end, NULL) < 0) {
            perror("gettimeofday");
        }

        {
            double rtt;
            long sec_diff = end.tv_sec - start.tv_sec;
            long usec_diff = end.tv_usec - start.tv_usec;
            rtt = sec_diff * 1000.0 + usec_diff / 1000.0;

            printf("%d: Sent... RTT=%f ms\n", i, rtt);

            if (received == 0) {
                min_rtt = max_rtt = rtt;
            } else {
                if (rtt < min_rtt) min_rtt = rtt;
                if (rtt > max_rtt) max_rtt = rtt;
            }
            sum_rtt += rtt;
            received++;
        }
    }

    {
        double loss = 0.0;
        double avg_rtt = 0.0;

        if (sent > 0) {
            loss = 100.0 * (double)(sent - received) / (double)sent;
        }
        if (received > 0) {
            avg_rtt = sum_rtt / (double)received;
        }

        printf("%d pkts xmited, %d pkts rcvd, %.0f%% pkt loss  ",
               sent, received, loss);

        if (received > 0) {
            printf("min: %f ms, max: %f ms, avg: %f ms\n",
                   min_rtt, max_rtt, avg_rtt);
        } else {
            printf("no RTT stats (0 packets received)\n");
        }
    }

    if (res) freeaddrinfo(res);
    close(sockfd);
    return 0;
}
