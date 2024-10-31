#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for Book
typedef struct {
    int id;
    char title[50];
    char author[50];
    char genre[20];
    int isBorrowed;
} Book;

// Structure for User
typedef struct User {
    char name[50];
    int userID;
    int borrowedCount;
    int borrowedBooks[3];  // Stores up to 3 borrowed book IDs for the user
    struct User* next;
} User;

#define MAX_BOOKS 100
#define BORROW_LIMIT 3  // Maximum books a user can borrow

Book books[MAX_BOOKS];
User* userHead = NULL;
int bookCount = 0;

// Function Prototypes
void initializeBooks();
void addUser(char* name, int userID);
User* findUser(int userID);
void registerMultipleUsers();
void addBook();
void displayBooks();
void borrowMultipleBooks();
void returnMultipleBooks();
void showPendingBooks(User* user);
void displayMenu();

int main() {
    int choice;
    printf("Welcome to our library!\n");  // Welcome message
    initializeBooks();  // Add pre-existing books

    // Main menu
    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // to clear the newline character from the buffer
        switch (choice) {
            case 1:
                registerMultipleUsers();
                break;
            case 2:
                borrowMultipleBooks();
                break;
            case 3:
                returnMultipleBooks();
                break;
            case 4:
                addBook();
                break;
            case 5:
                displayBooks();
                break;
            case 6:
                printf("Thanks for visiting our library, visit again!\n");  // Closing message
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Function Definitions

// Initialize pre-existing books
void initializeBooks() {
    Book initialBooks[10] = {
        {101, "To Kill a Mockingbird", "Harper Lee", "Fiction", 0},
        {102, "1984", "George Orwell", "Dystopian", 0},
        {103, "Pride and Prejudice", "Jane Austen", "Romance", 0},
        {104, "The Great Gatsby", "F. Scott Fitzgerald", "Classic", 0},
        {105, "Moby Dick", "Herman Melville", "Adventure", 0},
        {106, "The Catcher in the Rye", "J.D. Salinger", "Fiction", 0},
        {107, "War and Peace", "Leo Tolstoy", "Historical", 0},
        {108, "The Odyssey", "Homer", "Epic", 0},
        {109, "The Hobbit", "J.R.R. Tolkien", "Fantasy", 0},
        {110, "Hamlet", "William Shakespeare", "Tragedy", 0}
    };
    for (int i = 0; i < 10; i++) {
        books[bookCount++] = initialBooks[i];
    }
    printf("Pre-existing books added to the library.\n");
}

// Register multiple users in one go
void registerMultipleUsers() {
    char name[50];
    int userID;

    printf("Enter user details (type '0' as name to stop):\n");
    while (1) {
        printf("Enter User Name (or '0' to stop): ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0; // remove newline character

        if (strcmp(name, "0") == 0) {
            break;  // Exit if '0' is entered
        }

        printf("Enter 6-digit User ID (e.g., 123456): ");
        scanf("%d", &userID);
        getchar(); // to clear newline character

        addUser(name, userID);
    }
}

void addUser(char* name, int userID) {
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->name, name);
    newUser->userID = userID;
    newUser->borrowedCount = 0;  // Initialize borrowedCount to 0
    memset(newUser->borrowedBooks, -1, sizeof(newUser->borrowedBooks)); // Initialize borrowedBooks array
    newUser->next = userHead;
    userHead = newUser;
    printf("User %s registered successfully with ID %d!\n", name, userID);
}

User* findUser(int userID) {
    User* temp = userHead;
    while (temp != NULL) {
        if (temp->userID == userID) return temp;
        temp = temp->next;
    }
    return NULL;
}

void addBook() {
    Book newBook;
    printf("Enter Book ID (a unique number, e.g., 111): ");
    scanf("%d", &newBook.id);
    getchar(); // to clear newline character

    printf("Enter Book Title: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // remove trailing newline

    printf("Enter Author Name: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0; // remove trailing newline

    printf("Enter Genre (e.g., Fiction, Science): ");
    scanf("%s", newBook.genre);
    getchar(); // to clear newline character

    newBook.isBorrowed = 0;
    books[bookCount++] = newBook;
    printf("Book '%s' added successfully!\n", newBook.title);
}

void displayBooks() {
    printf("\nAvailable Books:\n");
    for (int i = 0; i < bookCount; i++) {
        printf("ID: %d, Title: %s, Author: %s, Genre: %s, %s\n",
               books[i].id, books[i].title, books[i].author, books[i].genre,
               books[i].isBorrowed ? "Not Available" : "Available");
    }
}

// Display pending books for a user
void showPendingBooks(User* user) {
    if (user->borrowedCount > 0) {
        printf("You have the following books pending return:\n");
        for (int i = 0; i < BORROW_LIMIT; i++) {
            if (user->borrowedBooks[i] != -1) {
                printf("Book ID: %d\n", user->borrowedBooks[i]);
            }
        }
    }
}

// Borrow multiple books with a limit of 3 per user
void borrowMultipleBooks() {
    int userID, bookID;
    printf("Enter your 6-digit User ID (e.g., 123456): ");
    scanf("%d", &userID);
    getchar(); // clear newline character
    User* user = findUser(userID);

    if (user != NULL) {
        printf("Welcome, %s!\n", user->name);
        if (user->borrowedCount >= BORROW_LIMIT) {
            printf("You have reached the borrowing limit of %d books.\n", BORROW_LIMIT);
            showPendingBooks(user);
            return;
        }

        displayBooks();
        printf("Enter Book IDs to borrow (separate multiple IDs with spaces, e.g., 101 102). Type -1 to finish:\n");

        while (1) {
            scanf("%d", &bookID);
            if (bookID == -1) break;

            if (user->borrowedCount >= BORROW_LIMIT) {
                printf("You have reached the borrowing limit of %d books.\n", BORROW_LIMIT);
                showPendingBooks(user);
                break;
            }

            int found = 0;
            for (int i = 0; i < bookCount; i++) {
                if (books[i].id == bookID && !books[i].isBorrowed) {
                    books[i].isBorrowed = 1;
                    user->borrowedBooks[user->borrowedCount++] = bookID;
                    printf("Book ID %d borrowed successfully!\n", bookID);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Book ID %d is either not available or already borrowed.\n", bookID);
            }
        }
        showPendingBooks(user);
    } else {
        printf("User not found. Please register first.\n");
    }
}

// Return multiple books
void returnMultipleBooks() {
    int userID, bookID;
    printf("Enter your 6-digit User ID (e.g., 123456): ");
    scanf("%d", &userID);
    getchar(); // clear newline character
    User* user = findUser(userID);

    if (user != NULL) {
        printf("Enter Book IDs to return (separate multiple IDs with spaces, e.g., 101 102). Type -1 to finish:\n");

        while (1) {
            scanf("%d", &bookID);
            if (bookID == -1) break;

            int found = 0;
            for (int i = 0; i < bookCount; i++) {
                if (books[i].id == bookID && books[i].isBorrowed) {
                    books[i].isBorrowed = 0;
                    for (int j = 0; j < BORROW_LIMIT; j++) {
                        if (user->borrowedBooks[j] == bookID) {
                            user->borrowedBooks[j] = -1;
                            user->borrowedCount--;
                            printf("Book ID %d returned successfully!\n", bookID);
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (!found) {
                printf("Book ID %d is either not found or not currently borrowed.\n", bookID);
            }
        }
        showPendingBooks(user);
    } else {
        printf("User not found. Please register first.\n");
    }
}

void displayMenu() {
    printf("\nLibrary Management System\n");
    printf("1. Register User\n");
    printf("2. Borrow Books\n");
    printf("3. Return Books\n");
    printf("4. Add New Book\n");
    printf("5. Display Books\n");
    printf("6. Exit\n");
}
