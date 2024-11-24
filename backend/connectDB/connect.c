#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HOST "localhost"
#define USER "root"
#define PASS "12345678"
#define DBNAME "ticketrail"
#define MAX_LENGTH 255

typedef struct {
    int id;
    char name[MAX_LENGTH];
    char description[MAX_LENGTH];
    char contact_email[MAX_LENGTH];
    char contact_phone[MAX_LENGTH];
    char logo_url[MAX_LENGTH]

} Airline;

int fetch_users(Airline **airline, int *count) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    if (mysql_real_connect(conn, HOST, USER, PASS, DBNAME, 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn, "SELECT * FROM airlines")) {
        fprintf(stderr, "SELECT failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    int num_rows = mysql_num_rows(res);
    *count = num_rows;
    *airline= malloc(sizeof(Airline) * num_rows);

    int i = 0;
    while ((row = mysql_fetch_row(res))) {
        (*airline)[i].id = atoi(row[0]);
        strncpy((*airline)[i].name, row[1], sizeof((*airline)[i].name) - 1);
        strncpy((*airline)[i].description, row[2], sizeof((*airline)[i].description) - 1);
        strncpy((*airline)[i].contact_email, row[3], sizeof((*airline)[i].contact_email) - 1);
        strncpy((*airline)[i].contact_phone, row[4], sizeof((*airline)[i].contact_phone) - 1);
        strncpy((*airline)[i].logo_url, row[5], sizeof((*airline)[i].logo_url) - 1);
        i++;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}

int main() {
    Airline *airline = NULL;
    int count = 0;

    if (fetch_users(&airline, &count) != 0) {
        fprintf(stderr, "Failed to fetch airlines.\n");
        return 1;
    }

    printf("Fetched %d airlines:\n", count);
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Description: %s, Contact email: %s, Contact phone: %s, Logo url: %s\n", airline[i].id, airline[i].name,airline[i].description, airline[i].contact_email, airline[i].contact_phone, airline[i].logo_url);
    }

    free(airline);

    return 0;
}
