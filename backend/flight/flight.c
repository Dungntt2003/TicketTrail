#include "flight.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fetch_flights(Flight **flights, int *count) {
    MYSQL *conn = connect_db();
    if (conn == NULL) {
        return 1;
    }

    if (mysql_query(conn, "select A.flight_id, A.flight_number, A.departure_time, B.name from (select * from ticketrail.flights join ticketrail.airplanes using (airplane_id)) as A join ticketrail.airlines as B using (airline_id)")) {
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
    *flights = malloc(sizeof(Flight) * num_rows);

    for (int i = 0; i < num_rows; i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        (*flights)[i].id = atoi(row[0]);
        strncpy((*flights)[i].flight_number, row[1], sizeof((*flights)[i].flight_number) - 1);
        strncpy((*flights)[i].departure_time, row[2], sizeof((*flights)[i].departure_time) - 1);
        strncpy((*flights)[i].name, row[3], sizeof((*flights)[i].name) - 1);
    }

    mysql_free_result(res);
    disconnect_db(conn);

    return 0;
}