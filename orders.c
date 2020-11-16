#include <stdio.h>
#include <stdlib.h>
#include "orders.h"
#include "menu.h"
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

int ShowOrderSubMenu();
void OpenQuery();
void RangeQuery();
void DetailQuery();


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
    char buf[16] = "\0";

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


void OpenQuery(){
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret, ret2; /* ODBC API return status */
    SQLSMALLINT columns = 1; /* number of columns in result-set */
    SQLUSMALLINT ucolumns = 0;
    SQLUSMALLINT i = 0;
    SQLINTEGER x = 0;
    #define BufferLength 512
    int flag = 0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }
    /* simple query */
    ret = SQLExecDirect(stmt, (SQLCHAR*) "select ordernumber from orders where shippeddate is NULL order by ordernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }
    /* How many columns are there */
    ret = SQLNumResultCols(stmt, &columns);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }

    /* print the name of each column */
    ucolumns = (SQLUSMALLINT)columns;
    for (i = 1; i <= ucolumns; i++) {
        char buf[BufferLength] = "\0";
        ret = SQLDescribeCol(stmt, i, (SQLCHAR *) buf, BufferLength, NULL, NULL, NULL, NULL, NULL);
        if (!SQL_SUCCEEDED(ret)) {
            odbc_extract_error("Error in SQLDescribeCol", stmt, SQL_HANDLE_STMT);
            return;
        }
        printf("%s\t", buf);
    }
    printf("\n");

    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &x, 0, NULL);
    
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        flag = 1;
        printf("%d\n", x);
    }

    if (flag == 0){
        printf("ERROR en los datos de entrada o consulta vacía");
        printf("\n\n\n");
    }

    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }
    printf("\n\n\n");

    return;
}



void RangeQuery() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    
    char fecha1[11] = "\0";
    char fecha2[11] = "\0";
    long ordernumber = 0;
    char shippeddate[11] = "\0";
    char orderdate[11] = "\0";
    int flag = 0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select ordernumber, orderdate, shippeddate from orders where orderdate between ? and ? order by ordernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }
    
    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
if(scanf("%s - %s", fecha1, fecha2) != EOF){
    
    
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, fecha1, 0, NULL);
    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, fecha2, 0, NULL);
    
    (void) SQLExecute(stmt);
        
    (void) SQLBindCol(stmt, 1, SQL_C_LONG, &ordernumber, 0, NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLDATE*) orderdate, (SQLLEN) sizeof(orderdate), NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLDATE*) shippeddate,(SQLLEN) sizeof(shippeddate), NULL);

    /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            flag = 1;
            printf("%ld %s %s\n", ordernumber,orderdate,shippeddate);
        }

        (void) SQLCloseCursor(stmt);
}
    (void) getchar();
    printf("\n");

    if (flag == 0){
        printf("ERROR en los datos de entrada o consulta vacía");
        printf("\n\n\n");
    }

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    return;
}


void DetailQuery() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    int  x = 0;
    char status[15] = "\0";
    char fecha[11] = "\0";
    float total = 0;
    char pcode[15] = "\0";
    int quantity = 0;
    float price = 0;
    int flag = 0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select orderdate, status from orders where ordernumber = ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }


    printf("Enter ordernumber > ");
    (void) fflush(stdout);
    if(scanf("%d", &x) != EOF) {
        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &x, 0, NULL);
        
        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) fecha,(SQLLEN) sizeof(fecha), NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *) status,(SQLLEN) sizeof(status), NULL);

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            flag = 1;
            printf("Order Date=%s - status=%s\n", fecha, status);
        }

        (void) SQLCloseCursor(stmt);

    }
    (void) getchar();
    printf("\n");

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }


    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select sum(quantityordered * priceeach) as total from orderdetails where ordernumber = ? group by ordernumber order by ordernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }


    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &x, 0, NULL);
        
    (void) SQLExecute(stmt);
        
    (void) SQLBindCol(stmt, 1, SQL_C_FLOAT, &total, 0, NULL);

        /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Total sum = %f\n", total);
    }

    (void) SQLCloseCursor(stmt);
    
    printf("\n");

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }


     /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select productcode, quantityordered, priceeach from orderdetails where ordernumber = ? order by orderlinenumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return;
    }


        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &x, 0, NULL);
        
        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) pcode,(SQLLEN) sizeof(pcode), NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_LONG, &quantity, 0, NULL);
        (void) SQLBindCol(stmt, 3, SQL_C_FLOAT, &price, 0, NULL);

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("%s %d %.2f\n", pcode, quantity, price);
        }

        (void) SQLCloseCursor(stmt);


    
   

    if (flag == 0){
        printf("ERROR en los datos de entrada o consulta vacía");
        printf("\n\n\n");
    }

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return;
    }

    return;
}
