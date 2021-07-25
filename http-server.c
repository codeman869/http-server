#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define PROTOCOL "HTTP/1.1"
#define HTTP_OK "200 OK"
#define HTTP_UNAUTHORIZED "401 Unauthorized"
#define HTTP_NOT_FOUND "404 Not Found"
#define HEADER_CONTEXT_TYPE "Content-Type: "
#define CONTEXT_TYPE_TEXT "Text/Plain\n"
#define HEADER_CONTENT_LENGTH "Content-Length: "
#define BODY_SEPERATOR "\n\n"

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof address;
    char *message = "Hello World! This is a custom built HTTP Server!\n";

    char *response; //= "HTTP/1.1 200 OK\nContent-Type:Text/Plain\nContent-Length: 12\n\nHello World!";
    int body_size = strlen(message);
    char body_size_char[4];
    sprintf(body_size_char, "%d", body_size);
    int header_size = strlen(PROTOCOL) + strlen(HTTP_OK)+strlen(HEADER_CONTEXT_TYPE) + strlen(CONTEXT_TYPE_TEXT) +
                        strlen(HEADER_CONTENT_LENGTH) + body_size + strlen(BODY_SEPERATOR) + 4;

    response = (char *) malloc( header_size * sizeof (char));
    strcpy(response, PROTOCOL);
    strcat(response, " ");
    strcat(response, HTTP_OK);
    strcat(response, "\n");
    strcat(response, HEADER_CONTEXT_TYPE);
    strcat(response, CONTEXT_TYPE_TEXT);
    strcat(response, HEADER_CONTENT_LENGTH);
    strcat(response, body_size_char);
    strcat(response, BODY_SEPERATOR);
    strcat(response, message);
    printf("The standard response is:\n%s\n\n", response);
    printf("Size of response is: %i\n\n", strlen(response));

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

        write(new_socket, response, strlen(response));

        printf("------------------response message sent-------------------\n");

        close(new_socket);
    }

    return 0;
}
