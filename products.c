#include <stdio.h>
#include <stdlib.h>
#include "products.h"



int showProductMenu() {
    int nChoice = 0;

    do {
        nChoice = ShowProductSubMenu();
        switch (nChoice) {

            case 1: {
                PrintMary();
            }
                break;

            case 2: {
                PrintJack();
            }
                break;

            case 3: {
                PrintLittleBoPeep();
            }
                break;

            case 4: {
                printf("Bye Bye\n\n");
            }
                break;
        }
    } while (nChoice != 4);
}


int ShowProductSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Mary had a little lamb\n"
               " (2) Jack and Jill\n"
               " (3) Little Bo Peep\n"
               " (4) Quit\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}



















