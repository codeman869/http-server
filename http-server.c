#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof address;

    char *hello = "HTTP/1.1 200 OK\nContent-Type:Text/Plain\nContent-Length: 12\n\nHello World!";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (const struct sockaddr* ) &address, sizeof address) < 0)
    {
        perror("Error in bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("In Listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, &addrlen)) < 0)
        {
            perror("In Accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, sizeof buffer);

        printf("%s\n", buffer);

        write(new_socket, hello, strlen(hello));

        printf("------------------Hello message sent-------------------\n");

        close(new_socket);
    }

    return 0;
}
