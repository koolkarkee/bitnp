#include<unistd.h>
#include<stdio.h>

//Wrapper function
int Execl(const char *__path, const char *_arg, ...){
    //execl call
    int result = execl(__path, _arg, (char *)0);
    if(result == -1)
    {
        perror("execl failed - ");
    }
}

int main(){  
    printf("\n1 Hello world from main function of calling program\n");     
    Execl("/home/koolkarkee/bitnp/two/mybashscript.sh", "mybashscript.sh", (char *)0);
        
    printf("\n2 Hello world from main function of calling program...");     

    return 0;
}