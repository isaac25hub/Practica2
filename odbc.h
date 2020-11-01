#ifndef AUX_H
#define AUX_H

/* CONNECTION PARAMETERS, ADAPT TO YOUR SCENARIO */
#define CONNECTION_PARS "DRIVER=psqlodbca.so;DATABASE=classicmodels;SERVER=localhost;PORT=5432;UID=postgres;PWD=/home/eps/Escritorio/odbc-examples/odbc-examples;"

/* REPORT OF THE MOST RECENT ERROR USING HANDLE handle */
void odbc_extract_error(char *fn, SQLHANDLE handle, SQLSMALLINT type);

/* STANDARD CONNECTION PROCEDURE */
int odbc_connect(SQLHENV* env, SQLHDBC* dbc);

/* STANDARD DISCONNECTION PROCEDURE */
int odbc_disconnect(SQLHENV env, SQLHDBC dbc);

#endif
