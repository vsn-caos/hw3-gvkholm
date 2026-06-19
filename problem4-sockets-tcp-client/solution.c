#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ipv4_addr> <port>\n", argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[2])),
        .sin_addr.s_addr = inet_addr(argv[1]),
    };
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }
    int n;
    while (scanf("%d", &n) == 1) {
        if (write(sock, &n, sizeof(n)) < 0) {
            perror("write");
            close(sock);
            return 1;
        }
        int res;
        ssize_t r = read(sock, &res, sizeof(res));
        if (r < 0) {
            close(sock);
            return 1;
        }
        printf("%d\n", res);
        fflush(stdout);
    }
    close(sock);
    return 0;
}
