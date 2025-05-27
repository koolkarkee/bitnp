#include<stdio.h>

//normal function 
int add(int a, int b){
    return a + b;
}

//wrapper function of add
int Add(int a, int b){
    //call the normal function from the wrapper function
    return add(a,b);
}

//main function
int main(){
    int result1 = add(3,5);
    printf("\nResult is = %d\n", result1);

    //from wrapper function
    int result2 = Add(3,5);
    printf("\nResult from wrapper function is = %d\n", result2);

}
