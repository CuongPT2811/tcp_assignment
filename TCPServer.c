#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <regex.h>
#include <ctype.h>

#define USERNAME_PATTERN "^[A-Za-z0-9!@#$%^&*?.\\/]{6,50}$"
#define PASSWORD_PATTERN "^[A-Za-z0-9!@#$%^&*?.\\/]{6,50}$"

#define MAX_LENGTH 1024
#define PORT 5555
#define SA struct sockaddr

void communicate_function(int client_fd);
int validate_pattern(const char *input, const char *pattern);
int validate_password(const char *password);

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

int validate_password(const char *password)
{
    int has_upper = 0;
    int has_lower = 0;
    int has_digit = 0;
    int has_special = 0;
    for (int i = 0; password[i]; i++)
    {
        if (isupper(password[i])) has_upper = 1;
        else if (islower(password[i])) has_lower = 1;
        else if (isdigit(password[i])) has_digit = 1;
        else if (strchr("!@#$%^&*?.\\/", password[i])) has_special = 1;
    }

    return has_upper && has_lower && has_digit && has_special;
}

int validate_pattern(const char *input, const char *pattern)
{
    regex_t regex;
    int result;
    result = regcomp(&regex, pattern, REG_EXTENDED);
    if(result != 0)
    {
        fprintf(stderr, "Regex could not be compiled\n");
        return 0;
    }
    result = regexec(&regex, input, 0, NULL, 0);
    regfree(&regex);
    return !result;
}

