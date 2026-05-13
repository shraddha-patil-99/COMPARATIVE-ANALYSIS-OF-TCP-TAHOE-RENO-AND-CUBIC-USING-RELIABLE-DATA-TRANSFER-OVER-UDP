#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORT 8080
#define MAX_MSG 100
#define SIZE 256

struct Packet {
    int seq;
    char data[SIZE];
};

int main() {
    int sockfd;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    char messages[MAX_MSG][SIZE];
    int total = 0;

    printf("\nReliable Data Transfer Simulator\n");
    printf("with End-to-End Congestion Control\n\n");

    printf("Enter messages (blank line to stop):\n");

    while (1) {
        fgets(messages[total], SIZE, stdin);

        if (strcmp(messages[total], "\n") == 0)
            break;

        messages[total][strcspn(messages[total], "\n")] = 0;
        total++;
    }

    float prob;
    printf("\nEnter packet transmission probability (0.0 - 1.0): ");
    scanf("%f", &prob);

    sendto(sockfd, &prob, sizeof(prob), 0,
           (struct sockaddr*)&server, len);

    int algo;
    printf("\nSelect Congestion Control Algorithm\n");
    printf("1. TCP Tahoe\n");
    printf("2. TCP Reno\n");
    printf("3. TCP Cubic\n");
    printf("Enter choice: ");
    scanf("%d", &algo);

    sendto(sockfd, &algo, sizeof(algo), 0,
           (struct sockaddr*)&server, len);

    int base = 0;
    int nextseq = 0;
    int cwnd = 1;
    int ssthresh = 8;
    int retransmissions = 0;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    while (base < total) {

        while (nextseq < base + cwnd && nextseq < total) {

            struct Packet pkt;
            pkt.seq = nextseq;
            strcpy(pkt.data, messages[nextseq]);

            printf("\nSending Seq %d : %s\n", pkt.seq, pkt.data);

            sendto(sockfd, &pkt, sizeof(pkt), 0,
                   (struct sockaddr*)&server, len);

            nextseq++;
        }

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);

        if (activity > 0) {

            int ack;
            recvfrom(sockfd, &ack, sizeof(ack), 0,
                     (struct sockaddr*)&server, &len);

            printf("ACK Received: %d\n", ack);

            if (ack > base) {
                base = ack;

                if (cwnd < ssthresh)
                    cwnd *= 2;
                else {
                    if (algo == 3)
                        cwnd += (cwnd * 0.3);
                    else
                        cwnd += 1;
                }

                printf("Updated cwnd = %d\n", cwnd);
            }
        }
        else {

            printf("\nTimeout -> Congestion Detected\n");
            retransmissions++;

            ssthresh = cwnd / 2;
            if (ssthresh < 1)
                ssthresh = 1;

            if (algo == 1) {
                printf("TCP Tahoe Activated\n");
                cwnd = 1;
            }
            else if (algo == 2) {
                printf("TCP Reno Activated\n");
                cwnd = ssthresh;
            }
            else {
                printf("TCP Cubic Activated\n");
                cwnd = ssthresh + 2;
            }

            nextseq = base;
        }
    }

    gettimeofday(&end, NULL);

    double start_time = start.tv_sec + start.tv_usec / 1000000.0;
    double end_time = end.tv_sec + end.tv_usec / 1000000.0;
    double time_taken = end_time - start_time;

    float efficiency = ((float)(total - retransmissions) / total) * 100;

    printf("\n=========== RESULT ===========\n");
    printf("Total Messages       : %d\n", total);
    printf("Retransmissions      : %d\n", retransmissions);
    printf("Time Taken           : %.2f sec\n", time_taken);
    printf("Efficiency           : %.2f %%\n", efficiency);

    FILE *fp = fopen("results.csv", "a");

    if (fp != NULL) {
        fprintf(fp, "%f,%d,%.2f,%.2f,%d\n",
                prob,
                retransmissions,
                time_taken,
                efficiency,
                algo);

        fclose(fp);
    }

    close(sockfd);

    return 0;
}
