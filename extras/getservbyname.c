#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main() {
    const char *service_name = "http";  // Try changing to "ftp", "ssh", etc.
    const char *protocol = "tcp";

    // Get service entry
    struct servent *service = getservbyname(service_name, protocol);

    if (service == NULL) {
        fprintf(stderr, "Service '%s' with protocol '%s' not found.\n", service_name, protocol);
        return 1;
    }

    // Display service info
    printf("Service Name   : %s\n", service->s_name);
    printf("Port Number    : %d\n", ntohs(service->s_port));  // Convert from network byte order
    printf("Protocol       : %s\n", service->s_proto);

    // Display aliases
    printf("Aliases        : ");
    char **alias = service->s_aliases;
    if (*alias == NULL) {
        printf("None\n");
    } else {
        while (*alias) {
            printf("%s ", *alias);
            alias++;
        }
        printf("\n");
    }

    return 0;
}
