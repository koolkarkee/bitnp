#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct hostent *host;
    struct in_addr **addr_list;

    host = gethostbyname(argv[1]);

    if (host == NULL) {
        herror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    printf("Official name: %s\n", host->h_name);

    addr_list = (struct in_addr **)host->h_addr_list;

    for (int i = 0; addr_list[i] != NULL; i++) {
        printf("IP Address: %s\n", inet_ntoa(*addr_list[i]));
    }

    return 0;
}
