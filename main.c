#include <stdlib.h>
#include <stdio.h>
#include "products.h"
#include "menu.h"


/**
 * @brief Entry point, shows and process main menu.
 *
 * This function implements a loop that prints the main
 * menu, analyze the user selection and calls the
 * corresponding submenu
 *
 * @return 0 if no error
 * @author rmarabini
 */
int main(void) {
    int nChoice = 0;

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1: {
                ShowProductMenu();
            }
                break;

            case 2: {
                ShowOrderMenu();
            }
                break;
            
            case 3: {
                /**/
            }
                break;

            case 4: {
                printf("Exiting menu...\n\n");
            }
                break;
        }
    } while (nChoice != 4);
    return 0;
}