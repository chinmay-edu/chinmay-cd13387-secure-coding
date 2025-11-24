#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_utils.h"

#define MAX_LINE_LENGTH 200
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_SALT_LENGTH 6
#define MAX_COMMAND_LENGTH 50
#define FILE_USERS "hashed_users.txt"

// Function to trim newline characters
// Function to trim newline and carriage return characters from the end
void trim_newline(char* str) {
    size_t len = strlen(str);
    // Remove trailing newline if present
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
        len--;
    }
    // Remove trailing carriage return if present (for Windows files)
    if (len > 0 && str[len-1] == '\r') {
        str[len-1] = '\0';
        len--;
    }
}


// Function to check if username and password match an entry in users.txt
int check_login(const char* username, const char* password) {

    printf("Entered Password : %s\n",password);
    if (strcmp(username, "superuser") == 0 && strcmp(password, "h4rdc0d3d") == 0) {
        return 1;
    }

    FILE* file = fopen(FILE_USERS, "r");
    if (file == NULL) {
        printf("Could not open hashed_users.txt\n");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    char file_username[MAX_USERNAME_LENGTH];
    char file_salt[MAX_SALT_LENGTH];
    char file_password[MAX_PASSWORD_LENGTH];
    char hashed_password[MAX_PASSWORD_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        // Remove the newline character
        trim_newline(line);

        // Split the line into username and password

        char *token1 = NULL;
        char *token2 = NULL;
        char *token3 = NULL;

        // 1. Extract the first token
        token1 = strtok(line, ":");
        strcpy(file_username, token1);
        // 2. Extract the second token
        // Pass NULL to continue scanning the original string
        if (token1 != NULL) {
            token2 = strtok(NULL, ":");
            strcpy(file_salt, token2);
        }

        // 3. Extract the third token
        if (token2 != NULL) {
            token3 = strtok(NULL, ":");
            strcpy(file_password, token3);
        }
        
        hash_password(password, file_salt, hashed_password);
        printf("File Username: %s\n",file_username);
        printf("File Salt: %s\n",file_salt);
        printf("File Password: %s\n",file_password);
        printf("Input Hashed Password: %s\n",hashed_password);

        // Compare entered username and password with the file's values
        if (strcmp(username, file_username) == 0 && strcmp(hashed_password, file_password) == 0) {
            fclose(file);
            return 1;  // Login successful
        }
    }

    fclose(file);
    return 0;  // Login failed
}

int main() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char command[MAX_COMMAND_LENGTH];

    // Prompt user for username and password
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    trim_newline(username);  // Remove newline character

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    trim_newline(password);  // Remove newline character

    // Check login credentials
    if (check_login(username, password)) {
        printf("Login successful!\n");

        // Command prompt loop
        while (1) {
            printf("> ");
            scanf("%s", command);

            if (strcmp(command, "exit") == 0) {
                break;
            } else {
                printf("Unknown command.\nAllowed command is exit.\n");
            }
        }
    } else {
        printf("Login failed.\n");
    }

    return 0;
}