#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_USERS 10
#define CREDENTIAL_LENGTH 30

typedef struct {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
} user;

user users[MAX_USERS];
int user_count = 0;

void register_user();
int login_user();
void fix_fgets_input(char* string);
void input_credentials(char* username, char* password);

int main() {
    int option;
    int user_index;

    while (1) {
        printf("\n Welcome to the User Management Hub \n(⁠◍⁠•⁠ᴗ⁠•⁠◍⁠)\n");
        printf("\n Please select an option from the menu below:\n");
        printf("\n 1. Register");
        printf("\n 2. Login");
        printf("\n 3. Exit");
        printf("\n\n Select an option: ");
        scanf("%d", &option);
        getchar(); // Clear the newline from the input buffer

        switch (option) {
            case 1:
                register_user();
                break;
            case 2:
                user_index = login_user();
                if (user_index >= 0) {
                    printf("\n Login Successful! \n Welcome, %s! \n", users[user_index].username);
                } else {
                    printf("\n Invalid username or password. Please try again. \n");
                }
                break;
            case 3:
                printf("\n Thank you for using the User Management Hub. Goodbye! \n");
                return 0;
            default:
                printf("\n Invalid option. Please try again.\n");
                break;
        }
    }
    return 0;
}

void register_user() {
    if (user_count == MAX_USERS) {
        printf("\n Maximum %d users are supported! No more registrations allowed!\n", MAX_USERS);
        return;
    }

    int new_index = user_count;
    printf("\nRegister a new user\n");

    input_credentials(users[new_index].username, users[new_index].password);

    user_count++;
    printf("\nRegistration Successful!\n");
}

int login_user() {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];

    input_credentials(username, password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            return i;
        }
    }

    return -1;
}

void input_credentials(char* username, char* password) {
    printf("\nEnter Username: ");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_fgets_input(username);

    printf("Enter Password: ");
    fflush(stdout);

    struct termios old_props, new_props;
    tcgetattr(STDIN_FILENO, &old_props);
    new_props = old_props;
    new_props.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);

    char ch;
    int i = 0;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (ch == '\b' || ch == 127) { // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < CREDENTIAL_LENGTH - 1) { // Ensure no buffer overflow
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
    printf("\n");
}

void fix_fgets_input(char* string) {
    int index = strcspn(string, "\n");
    string[index] = '\0';
}


