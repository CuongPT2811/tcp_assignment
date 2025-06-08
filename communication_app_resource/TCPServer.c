#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>



#define MAX_LENGTH 1024
#define PORT 5555
#define SA struct sockaddr

void communicate_function(int client_fd);

int main()
{
    int server_fd;
    int client_fd;
    int client_address_length;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        printf("Socket creation failed\n");
        exit(0);
    }
    else
    {
        printf("Socket creation successful\n");
    }
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if(bind(server_fd, (SA*)&server_address, sizeof(server_address))!=0)
    {
        printf("Socket bind failed\n");
        exit(0);
    }
    else
    {
        printf("Socket bind successfully\n");
    }

    if((listen(server_fd, 5))!=0)
    {
        printf("Failed to listen\n");
        exit(0);
    }
    else
    {
        printf("Server is listening...\n");
    }

    client_address_length = sizeof(client_address);

    client_fd = accept(server_fd, (SA*)&client_address, &client_address_length);
    if(client_fd<0)
    {
        printf("Server failed to accept");
        exit(0);
    }
    else
    {
        printf("Server has accepted the client!\n");
    }

    communicate_function(client_fd);
    close(client_fd);
}

void communicate_function(int client_fd)
{
    char message[MAX_LENGTH];

    while(1)
    {
        memset(message, 0, MAX_LENGTH);
        read(client_fd, message, sizeof(message));
        printf("From client: %s \n", message);
        if((strncmp(message, "exit", 4)) == 0)
        {
            printf("Exit request from client....\n");
            break;
        }

        memset(message, 0, MAX_LENGTH);
        printf("To client: ");
        fgets(message, MAX_LENGTH, stdin);
        write(client_fd, message, strlen(message));
        if((strncmp("exit", message, 4)) == 0)
        {
            printf("Exit request from server...\n");
            break;
        }
    }
}



