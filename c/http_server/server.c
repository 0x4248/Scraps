/* SPDX-License-Identifier: GPL-3.0
 * http_server
 *
 * server.c
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2024 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 * 
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024


int main() {
    int server_fd, new_socket;
    char buffer[BUFFER_SIZE] = {0};

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return -1;
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    printf("Simple HTTP server is running on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&address)) < 0) {
            perror("Accept failed");
            close(server_fd);
            return -1;
        }

        read(new_socket, buffer, BUFFER_SIZE);
        printf("Received request:\n%s\n", buffer);

        char *path = strtok(buffer, " ");
        path = strtok(NULL, " ");

        printf("Requested path: %s\n", path);

        char *html_response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<!DOCTYPE html><html><head><title>Simple HTTP Server</title></head><body><h1>Hello, World!</h1></body></html>";

        write(new_socket, html_response, strlen(html_response));

        close(new_socket);
    }

    return 0;
}