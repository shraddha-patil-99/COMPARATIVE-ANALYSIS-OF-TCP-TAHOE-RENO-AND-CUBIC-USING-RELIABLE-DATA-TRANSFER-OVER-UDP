#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8080
#define SIZE 256

struct Packet {
    int seq;
    char data[SIZE];
};

int main() {

    int sockfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));

    srand(time(NULL));

    printf("Receiver Started...\n");

    float prob;

    recvfrom(sockfd, &prob, sizeof(prob), 0,
             (struct sockaddr*)&client, &len);

    int algo;

    recvfrom(sockfd, &algo, sizeof(algo), 0,
             (struct sockaddr*)&client, &len);

    printf("Transmission Probability : %.2f\n", prob);

    if (algo == 1)
        printf("Current Algorithm : TCP Tahoe\n");
    else if (algo == 2)
        printf("Current Algorithm : TCP Reno\n");
    else
        printf("Current Algorithm : TCP Cubic\n");

    int expected = 0;

    while (1) {

        struct Packet pkt;

        recvfrom(sockfd, &pkt, sizeof(pkt), 0,
                 (struct sockaddr*)&client, &len);

        float random_prob = (float)rand() / RAND_MAX;

        if (random_prob > prob) {
            printf("\nPacket %d LOST\n", pkt.seq);

            sendto(sockfd, &expected, sizeof(expected), 0,
                   (struct sockaddr*)&client, len);

            continue;
        }

        printf("\nReceived Seq %d : %s\n", pkt.seq, pkt.data);

        if (pkt.seq == expected) {
            expected++;
        }

        sendto(sockfd, &expected, sizeof(expected), 0,
               (struct sockaddr*)&client, len);

        printf("ACK Sent : %d\n", expected);
    }

    close(sockfd);

    return 0;
}
