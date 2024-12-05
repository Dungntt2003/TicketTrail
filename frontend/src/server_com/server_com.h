#ifndef SERVER_COM_H
#define SERVER_COM_H

#include <stddef.h>

int connect_to_server(const char *server_ip, int port);

void send_to_server(int sock, const void *data, size_t size);

void recv_from_server(int sock, void *buffer, size_t size);

#endif 
