#include <stdlib.h>
#include <stdio.h>



void main(){

    int choice;
    
    printf("1. Products\n");
    printf("2. Orders\n");
    printf("3. Customers\n");
    printf("4. Exit");
    ptintf("Enter a number that corresponds to your choice >");
    scanf("%d",&choice);
    printf("\n");

    if(choice == 4){
        return;
    }
    else if(choice == 1){
        printf("This is products menu:\n");
    }
    else if(choice == 2){
        printf("This is orders menu:");
    }
    else if(choice == 3){
        printf("This is customers menu:");
    }
    else{
        return;
    }
}