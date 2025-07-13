#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <IP address>\n", argv[0]);
        return 1;
    }

    struct in_addr addr;
    struct hostent *host;

    // Convert string IP to binary form
    if (!inet_aton(argv[1], &addr)) {
        fprintf(stderr, "Invalid IP address format.\n");
        return 1;
    }

    // Perform reverse lookup
    host = gethostbyaddr((const void *)&addr, sizeof(addr), AF_INET);

    if (host == NULL) {
        herror("gethostbyaddr");
        return 1;
    }

    printf("Official name: %s\n", (*host).h_name);

    char **alias = (*host).h_aliases;
    while (*alias != NULL) {
        printf("Alias: %s\n", *alias);
        alias++;
    }

    return 0;
}
