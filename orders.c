#include <stdio.h>
#include <stdlib.h>
#include "orders.h"
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"





void ShowOrderMenu() {
    int nChoice = 0;

    do {
        nChoice = ShowOrderSubMenu();
        switch (nChoice) {

            case 1: {
                OpenQuery();
            }
                break;

            case 2: {
                
                RangeQuery();
            }
                break;

            case 3: {
                DetailQuery();
            }
                break;

            case 4: 
                break;

        }
    } while (nChoice != 4);
}


int ShowOrderSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Open\n"
               " (2) Range\n"
               " (3) Detail\n"
               " (4) Back\n\n");

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