#include <stdio.h>
#include <stdlib.h>
#include "customers.h"
#include "menu.h"
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


int FindCustomersQuery();
int ListProductsQuery();
/*int BalanceQuery();*/


void ShowCustomerMenu() {
    int nChoice = 0;

    do {
        nChoice = ShowCustomerSubMenu();
        switch (nChoice) {

            case 1: {
                FindCustomersQuery();
            }
                break;

            case 2: {
                
               ListProductsQuery();
            }
                break;

            case 3: {
               /* BalanceQuery();*/
            }
                break;

            case 4: 
                break;

        }
    } while (nChoice != 4);
}


int ShowCustomerSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Find\n"
               " (2) List Products\n"
               " (3) Balance\n"
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

int FindCustomersQuery() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    char query[72] = "%";
    int customernumber;
    char contactfirstname[50];
    char contactlastname[50];
    char customername[50];
    char name[50];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select customernumber, contactfirstname, contactlastname, customername from customers where contactfirstname like ? or contactlastname like ? order by customernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }


    printf("Enter customer name > ");
    (void) fflush(stdout);
    if (scanf("%s", name) != EOF) {
        strcat(query, name);
        strcat(query, "%");

        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, query, 0, NULL);
        (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, query, 0, NULL);
        
        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_LONG, &customernumber, 0, NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_CHAR, contactfirstname, sizeof(contactfirstname), NULL);
        (void) SQLBindCol(stmt, 3, SQL_C_CHAR, contactlastname, sizeof(contactlastname), NULL);
        (void) SQLBindCol(stmt, 4, SQL_C_CHAR, customername, sizeof(customername), NULL);

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("%d %s %s %s\n", customernumber, customername, contactfirstname, contactlastname);
        }

        (void) SQLCloseCursor(stmt);

       
    }
    printf("\n");
    getchar();

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


int ListProductsQuery() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    int customernumber;
    long quantityordered = 0;
    char productname[70];
    

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select products.productname, sum(orderdetails.quantityordered) as total from orders,orderdetails,products where orders.ordernumber=orderdetails.ordernumber and orderdetails.productcode=products.productcode and orders.customernumber= ? group by products.productcode order by products.productcode", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }
    
    printf("Enter customer number > ");
    (void) fflush(stdout);
    if(scanf("%d", &customernumber) != EOF) {
        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &customernumber, 0, NULL);
        
        (void) SQLExecute(stmt);

        (void) SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) productname, sizeof(productname), NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_LONG, &quantityordered, 0, NULL);
        

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("%s %ld\n", productname, quantityordered);
        }

        (void) SQLCloseCursor(stmt);

    }
    

    getchar();    
    printf("\n");

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}