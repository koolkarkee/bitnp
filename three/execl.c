#include<unistd.h>
#include<stdio.h>

//Wrapper function
int Execl(const char *__path, const char *_arg, ...){
    //execl call
    int result = execl(__path, _arg, (char *)0);
    if(result == -1)
    {
        perror("execl failed");
    }
}

int main(){        
    Execl("/home/koolkarkee/bitnp/three/mybashscript.sh", "mybashscript.sh", (char *)0);

    return 0;
}