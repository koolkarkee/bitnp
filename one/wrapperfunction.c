#include<stdio.h>

//original function 
double divide(double a, double b){
    return a/b;
}

//wrapper function 
double Divide(double a, double b){
    if(b == 0){
        printf("Divide by Zero exception\n");
        return 0;
    }

    return divide(a, b);
}

int main(){
    //don't call the original function but only the wrapper function
    double result1 = Divide(20.5, 4);
    printf("\nThe result is : %f\n", result1);

    //prints error
    double result2 = Divide(20.5, 0);
    printf("\nThe result is : %f\n", result2);

    return 0;
}