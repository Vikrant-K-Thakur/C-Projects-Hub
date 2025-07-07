#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 100
#define MAX_USERS 100

struct Book {
    int bookID;
    char title[50];
    char author[50];
};

struct User {
    char username[30];
    char password[30];
    char role[20];  // admin, librarian, or user
};

// Global Variables
struct User users[MAX_USERS];
int userCount = 0;
struct User currentUser;
struct Book library[MAX_BOOKS];
int numBooks = 0;

// Function Prototypes
void printHeader();
void printMenu();
int login();
void addBook();
void displayBooks();
void deleteBook();
void searchBook();
void addUser();
void loadUsersFromFile();
void saveUsersToFile();
void loadBooksFromFile();
void saveBooksToFile();

void printHeader() {
    printf("\n====================================\n");
    printf("          Library System            \n");
    printf("====================================\n");
}

void printMenu() {
    printf("\nChoose an option:\n");
    printf("1. Add Book\n");
    printf("2. Display Books\n");
    printf("3. Delete Book\n");
    printf("4. Search Book by ID\n");
    printf("5. Exit\n");
    if (strcmp(currentUser.role, "admin") == 0 || strcmp(currentUser.role, "librarian") == 0)
        printf("6. Add New User\n");
    printf("====================================\n");
    printf("Enter your choice: ");
}

void loadUsersFromFile() {
    FILE *file = fopen("users.txt", "r");
    if (!file) return;

    while (fscanf(file, "%s %s %s", users[userCount].username, users[userCount].password, users[userCount].role) == 3) {
        userCount++;
    }
    fclose(file);
}

void saveUsersToFile() {
    FILE *file = fopen("users.txt", "w");
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %s\n", users[i].username, users[i].password, users[i].role);
    }
    fclose(file);
}

void loadBooksFromFile() {
    FILE *file = fopen("books.txt", "r");
    if (!file) return;

    while (fscanf(file, "%d\n", &library[numBooks].bookID) == 1) {
        fgets(library[numBooks].title, sizeof(library[numBooks].title), file);
        fgets(library[numBooks].author, sizeof(library[numBooks].author), file);
        numBooks++;
    }
    fclose(file);
}

void saveBooksToFile() {
    FILE *file = fopen("books.txt", "w");
    for (int i = 0; i < numBooks; i++) {
        fprintf(file, "%d\n%s%s", library[i].bookID, library[i].title, library[i].author);
    }
    fclose(file);
}

int login() {
    char uname[30], pword[30];
    int attempts = 3;

    while (attempts--) {
        printf("Login to Access Library System\n");
        printf("Username: ");
        scanf("%s", uname);
        printf("Password: ");
        scanf("%s", pword);

        for (int i = 0; i < userCount; i++) {
            if (strcmp(uname, users[i].username) == 0 &&
                strcmp(pword, users[i].password) == 0) {
                currentUser = users[i];
                printf("\nLogin successful! Welcome, %s.\n", currentUser.username);
                return 1;
            }
        }
        printf("Invalid credentials. Attempts left: %d\n", attempts);
    }

    printf("Too many failed attempts. Access denied.\n");
    return 0;
}

void addUser() {
    if (strcmp(currentUser.role, "admin") != 0 && strcmp(currentUser.role, "librarian") != 0) {
        printf("You do not have permission to add users.\n");
        return;
    }

    if (userCount >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    struct User newUser;
    printf("\nEnter new username: ");
    scanf("%s", newUser.username);

    // Check if username already exists
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("Username already exists. Cannot add user.\n");
            return;
        }
    }

    printf("Enter password: ");
    scanf("%s", newUser.password);
    printf("Enter role (admin/librarian/user): ");
    scanf("%s", newUser.role);

    users[userCount++] = newUser;
    saveUsersToFile();
    printf("User '%s' added successfully with role '%s'.\n", newUser.username, newUser.role);
}

void addBook() {
    if (numBooks >= MAX_BOOKS) {
        printf("Library is full. Cannot add more books.\n");
        return;
    }

    int newID;
    printf("\nEnter Book ID: ");
    scanf("%d", &newID);

    // Check for duplicate Book ID
    for (int i = 0; i < numBooks; i++) {
        if (library[i].bookID == newID) {
            printf("Book ID %d already exists. Cannot add duplicate.\n", newID);
            return;
        }
    }

    library[numBooks].bookID = newID;
    getchar(); // consume newline left by scanf

    printf("Enter Book Title: ");
    fgets(library[numBooks].title, sizeof(library[numBooks].title), stdin);
    printf("Enter Book Author: ");
    fgets(library[numBooks].author, sizeof(library[numBooks].author), stdin);
    numBooks++;
    saveBooksToFile();
    printf("Book added successfully!\n");
}

void displayBooks() {
    if (numBooks == 0) {
        printf("No books in the library.\n");
    } else {
        printf("\nList of Books:\n");
        printf("------------------------------------\n");
        for (int i = 0; i < numBooks; i++) {
            printf("Book ID   : %d\n", library[i].bookID);
            printf("Title     : %s", library[i].title);
            printf("Author    : %s", library[i].author);
            printf("------------------------------------\n");
        }
    }
}

void deleteBook() {
    if (numBooks == 0) {
        printf("No books in the library to delete.\n");
    } else {
        int id, found = 0;
        printf("\nEnter Book ID to delete: ");
        scanf("%d", &id);
        for (int i = 0; i < numBooks; i++) {
            if (library[i].bookID == id) {
                for (int j = i; j < numBooks - 1; j++) {
                    library[j] = library[j + 1];
                }
                numBooks--;
                found = 1;
                saveBooksToFile();
                printf("Book with ID %d deleted successfully!\n", id);
                break;
            }
        }
        if (!found) {
            printf("Book with ID %d not found.\n", id);
        }
    }
}

void searchBook() {
    if (numBooks == 0) {
        printf("No books in the library to search.\n");
    } else {
        int id, found = 0;
        printf("\nEnter Book ID to search: ");
        scanf("%d", &id);
        for (int i = 0; i < numBooks; i++) {
            if (library[i].bookID == id) {
                printf("\nBook Found:\n");
                printf("------------------------------------\n");
                printf("Book ID   : %d\n", library[i].bookID);
                printf("Title     : %s", library[i].title);
                printf("Author    : %s", library[i].author);
                printf("------------------------------------\n");
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Book with ID %d not found.\n", id);
        }
    }
}

int main() {
    loadUsersFromFile();
    loadBooksFromFile();

    if (!login()) return 0;

    int choice;

    while (1) {
        printHeader();
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                deleteBook();
                break;
            case 4:
                searchBook();
                break;
            case 5:
                printf("Exiting the system... Thank you!\n");
                return 0;
            case 6:
                addUser();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}