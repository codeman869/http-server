#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0;
    long valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof serv_addr);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid Address");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (const struct sockaddr *) &serv_addr, sizeof serv_addr) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    if (send(sock, hello, strlen(hello), 0) < 0)
    {
        perror("Error sending hello");
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent!\n");
    valread = read(sock, buffer, sizeof buffer);
    printf("%s\n", buffer);

    exit(EXIT_SUCCESS);
}