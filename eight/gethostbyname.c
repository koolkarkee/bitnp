#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    struct hostent *host;
    struct in_addr **addr_list;

    host = gethostbyname(argv[1]);

    if (host == NULL) {
        herror("gethostbyname");
        return 1;
    }

    printf("Official name: %s\n", (*host).h_name);

    addr_list = (struct in_addr **)((*host).h_addr_list);

    int i = 0;
    while (addr_list[i] != NULL) {
        printf("IP Address: %s\n", inet_ntoa(*addr_list[i]));
        i++;
    }

    return 0;
}
