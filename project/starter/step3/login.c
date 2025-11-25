#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_utils.h"

#define MAX_LINE_LENGTH 200
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 65
#define MAX_SALT_LENGTH 6
#define MAX_COMMAND_LENGTH 50
#define SALT_LENGTH 2
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
        // Remove the newline character if it exists at the end
        trim_newline(line);
        //printf("Line : %s", line);
        // We use a temporary pointer for robust token management within this loop iteration
        char *temp_token = NULL;

        // 1. Extract the username token
        temp_token = strtok(line, ":");
        if (temp_token == NULL) continue; // Skip malformed lines
        
        // Use strncpy for safety
        strncpy(file_username, temp_token, MAX_USERNAME_LENGTH - 1);
        file_username[MAX_USERNAME_LENGTH - 1] = '\0'; // Ensure null termination

        if (strcmp(username, file_username) == 0)
        {
            // We found the user. Now extract salt and password immediately.
            
            // 2. Extract the salt token
            temp_token = strtok(NULL, ":");
            if (temp_token == NULL) {
                printf("Error: Missing salt for user %s\n", file_username);
                continue; // Skip this user entry
            }
            strncpy(file_salt, temp_token, MAX_SALT_LENGTH - 1);
            file_salt[MAX_SALT_LENGTH - 1] = '\0';

            // 3. Extract the password hash token
            temp_token = strtok(NULL, ":");
            if (temp_token == NULL) {
                printf("Error: Missing password hash for user %s\n", file_username);
                continue; // Skip this user entry
            }
            strncpy(file_password, temp_token, MAX_PASSWORD_LENGTH - 1);
            file_password[MAX_PASSWORD_LENGTH - 1] = '\0';

            // Debug prints
            //printf("token1 (Username): %s\n", file_username);
            //printf("token2 (Salt): %s\n", file_salt);
            //printf("token3 (File Password): %s\n", file_password);

            trim_newline(file_salt);
            trim_newline(password);

            unsigned char file_salt_bytes[SALT_LENGTH]; // Use the SALT_LENGTH (2) from generation code

            // CONVERT THE HEX STRING BACK TO BYTES
            // You need a hex_to_bytes function (assuming it's in hash_utils.h)
            hex_to_bytes(file_salt, file_salt_bytes, SALT_LENGTH * 2); // Convert 4 hex chars to 2 bytes

            // Now, pass the correct binary salt bytes to the hashing function
            hash_password(password, file_salt_bytes, hashed_password);
            
            //printf("Input Hashed Password: %s\n", hashed_password);

            // Compare generated hash with stored hash
            if (strcmp(hashed_password, file_password) == 0) {
                fclose(file);
                return 1;  // Login successful
            }
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