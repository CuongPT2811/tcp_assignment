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

void communicate_function(int server_fd);
int validate_pattern(const char *input, const char *pattern);
int validate_password(const char *password);

int main()
{
    int server_fd;

    struct sockaddr_in server_address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        printf("Failed to create socket\n");
        exit(0);
    }
    else
    {
        printf("Socket created successfully\n");
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);

    if(connect(server_fd, (SA*)&server_address, sizeof(server_address))!=0)
    {
        printf("Failed to connect with the server\n");
        exit(0);
    }
    else
    {
        printf("Successfully connected to the server!\n");
    }

    communicate_function(server_fd);
    close(server_fd);

}

void communicate_function(int server_fd)
{
    char message[MAX_LENGTH];
    while(1)
    {
        memset(message, 0, sizeof(message));
        printf("To server: ");
        fgets(message, MAX_LENGTH, stdin);
        write(server_fd, message, strlen(message));
        if((strncmp("exit", message, 4)) == 0)
        {
            printf("Exit request from client....\n");
            break;
        }

        memset(message, 0, sizeof(message));
        read(server_fd, message, sizeof(message));
        printf("From server: %s \t", message);
        if((strncmp(message, "exit", 4)) == 0)
        {
            printf("Exit request from server....\n");
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
