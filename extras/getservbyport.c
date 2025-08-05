#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main() {
    int port_number = 80;  // Try changing to 21, 22, 53, etc.
    const char *protocol = "tcp";

    // Convert port to network byte order
    int net_port = htons(port_number);

    // Get service entry
    struct servent *service = getservbyport(net_port, protocol);

    if (service == NULL) {
        fprintf(stderr, "No service found for port %d with protocol %s.\n", port_number, protocol);
        return 1;
    }

    // Display service info
    printf("Service Name   : %s\n", service->s_name);
    printf("Port Number    : %d\n", ntohs(service->s_port));  // Convert back to host byte order
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