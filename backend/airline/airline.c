#include "airline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fetch_airlines(Airline **airlines, int *count) {
    MYSQL *conn = connect_db();
    if (conn == NULL) {
        return 1;
    }

    if (mysql_query(conn, "SELECT * FROM airlines")) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        disconnect_db(conn);
        return 1;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        disconnect_db(conn);
        return 1;
    }

    int num_rows = mysql_num_rows(res);
    *count = num_rows;
    *airlines = malloc(sizeof(Airline) * num_rows);

    for (int i = 0; i < num_rows; i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        (*airlines)[i].id = atoi(row[0]);
        strncpy((*airlines)[i].name, row[1], sizeof((*airlines)[i].name) - 1);
        strncpy((*airlines)[i].description, row[2], sizeof((*airlines)[i].description) - 1);
        strncpy((*airlines)[i].contact_email, row[3], sizeof((*airlines)[i].contact_email) - 1);
        strncpy((*airlines)[i].contact_phone, row[4], sizeof((*airlines)[i].contact_phone) - 1);
        strncpy((*airlines)[i].logo_url, row[5], sizeof((*airlines)[i].logo_url) - 1);
    }

    mysql_free_result(res);
    disconnect_db(conn);

    return 0;
}