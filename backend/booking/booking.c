#include "booking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int insert_booking(const char *booking_time, int total_price, int user_id, const char *flight_id, int *insert_id) {
    MYSQL *conn = connect_db();
    if (conn == NULL) {
        return 1;
    }
    
    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO bookings (booking_time, total_price, user_id, flight_id) "
             "VALUES ('%s', %d, %d, '%s')",
             booking_time, total_price, user_id, flight_id);

    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    
    *insert_id = mysql_insert_id(conn);
    if (*insert_id == 0) {
        fprintf(stderr, "Failed to retrieve LAST_INSERT_ID()\n");
        mysql_close(conn);
        return 1;
    }

    mysql_close(conn);
    return 0;
}



