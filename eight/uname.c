#include <sys/utsname.h>
#include <stdio.h>

int main(void)
{
    struct utsname uts;
    uname(&uts);

    printf( "Init: uts.sysname: %s\n", uts.sysname );
    printf( "Init: uts.nodename: %s\n", uts.nodename );
    printf( "Init: uts.release: %s\n", uts.release );
    printf( "Init: uts.version: %s\n", uts.version );
    printf( "Init: uts.machine: %s\n", uts.machine );

    puts("");

    return 0;
}