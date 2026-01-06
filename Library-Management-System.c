/* ==== LIBRARY MANAGEMENT SYSTEM ====
   --------------------------------------------
   Features:
   - Add, display, search, update, delete books
   - Issue and return books
   - File handling for permanent storage
   - Prevents duplicate Book IDs */

#include <stdio.h>
#include <string.h>

/* -------------- CONSTANT DEFINITIONS ----------- */
#define MAX 100     // Maximum number of books allowed

/* ------------ STRUCTURE DEFINITION ------------ */
struct Book {
    int id;                 // Unique Book ID
    char title[50];         // Book Title
    char author[50];        // Author Name
    int year;               // Publication Year
    int quantity;           // Number of copies
    int status;             // 0 = Available, 1 = Issued
};

/* -------------- GLOBAL VARIABLES ----------------- */
struct Book library[MAX];   // Array to store book records
int bookCount = 0;          // Current number of books

/* ------------ FUNCTION DECLARATIONS --------------- */
void loadFromFile();
void saveToFile();

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void updateBook();
void deleteBook();

void printMenu();

/* ================ MAIN FUNCTION ================== */
int main() {
    int choice;

    /* Load previously saved records from file */
    loadFromFile();

    do {
        printMenu();
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: updateBook(); break;
            case 7: deleteBook(); break;
            case 8:
                saveToFile();
                printf("Exiting... Data saved successfully.\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 8);

    return 0;
}

/* ================ FILE HANDLING ===================== */

/* Load book records from file at program start */
void loadFromFile() {
    FILE *fp = fopen("books.txt", "r");

    if (fp == NULL) {
        printf("No saved data found. Starting new database...\n");
        return;
    }

    while (fscanf(fp, "%d,%49[^,],%49[^,],%d,%d,%d\n",
                  &library[bookCount].id,
                  library[bookCount].title,
                  library[bookCount].author,
                  &library[bookCount].year,
                  &library[bookCount].quantity,
                  &library[bookCount].status) == 6) {
        bookCount++;
    }

    fclose(fp);
}

/* Save all book records to file */
void saveToFile() {
    FILE *fp = fopen("books.txt", "w");

    if (fp == NULL) {
        printf("Error saving file!\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(fp, "%d,%s,%s,%d,%d,%d\n",
                library[i].id,
                library[i].title,
                library[i].author,
                library[i].year,
                library[i].quantity,
                library[i].status);
    }

    fclose(fp);
}

/* ================= CORE FUNCTIONALITY =================== */

/* Add a new book to the library */
void addBook() {
    if (bookCount >= MAX) {
        printf("Library is full!\n");
        return;
    }

    int id;
    printf("Enter Book ID: ");
    scanf("%d", &id);

    /* Check for duplicate Book ID */
    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            printf("Book ID already exists! Try a different ID.\n");
            return;
        }
    }

    struct Book *b = &library[bookCount];
    b->id = id;

    printf("Enter Title: ");
    scanf(" %[^\n]", b->title);

    printf("Enter Author: ");
    scanf(" %[^\n]", b->author);

    printf("Enter Year: ");
    scanf("%d", &b->year);

    printf("Enter Quantity: ");
    scanf("%d", &b->quantity);

    b->status = 0;   // Book is available initially

    bookCount++;
    saveToFile();

    printf("Book added successfully!\n");
}

/* Display all books in the library */
void displayBooks() {
    if (bookCount == 0) {
        printf("No books available.\n");
        return;
    }

    printf("\n------------ BOOK LIST ------------\n");
    for (int i = 0; i < bookCount; i++) {
        printf("ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nQuantity: %d\nStatus: %s\n\n",
               library[i].id,
               library[i].title,
               library[i].author,
               library[i].year,
               library[i].quantity,
               (library[i].status == 0) ? "Available" : "Issued");
    }
}

/* Search for a book using Book ID */
void searchBook() {
    int id, found = 0;

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            printf("\nBook Found!\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nQuantity: %d\nStatus: %s\n",
                   library[i].id,
                   library[i].title,
                   library[i].author,
                   library[i].year,
                   library[i].quantity,
                   (library[i].status == 0) ? "Available" : "Issued");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book not found.\n");
}

/* Issue a book (reduce quantity) */
void issueBook() {
    int id;
    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            if (library[i].quantity > 0) {
                library[i].quantity--;

                if (library[i].quantity == 0)
                    library[i].status = 1;

                saveToFile();
                printf("Book issued successfully!\n");
            } else {
                printf("Book is not available!\n");
            }
            return;
        }
    }

    printf("Book not found.\n");
}

/* Return a book (increase quantity) */
void returnBook() {
    int id;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            library[i].quantity++;
            library[i].status = 0;

            saveToFile();
            printf("Book returned successfully!\n");
            return;
        }
    }

    printf("Book not found.\n");
}

/* Update book details */
void updateBook() {
    int id;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {

            printf("Enter new Title: ");
            scanf(" %[^\n]", library[i].title);

            printf("Enter new Author: ");
            scanf(" %[^\n]", library[i].author);

            printf("Enter new Year: ");
            scanf("%d", &library[i].year);

            printf("Enter new Quantity: ");
            scanf("%d", &library[i].quantity);

            library[i].status = (library[i].quantity > 0) ? 0 : 1;

            saveToFile();
            printf("Book updated successfully!\n");
            return;
        }
    }

    printf("Book not found.\n");
}

/* Delete a book record */
void deleteBook() {
    int id, index = -1;

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (library[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Book not found.\n");
        return;
    }

    for (int i = index; i < bookCount - 1; i++)
        library[i] = library[i + 1];

    bookCount--;
    saveToFile();

    printf("Book deleted successfully!\n");
}

/* =================== MENU DISPLAY ======================= */

void printMenu() {
    printf("\n========== LIBRARY MENU ==========\n");
    printf("1. Add Book\n");
    printf("2. Display Books\n");
    printf("3. Search Book\n");
    printf("4. Issue Book\n");
    printf("5. Return Book\n");
    printf("6. Update Book\n");
    printf("7. Delete Book\n");
    printf("8. Exit\n");
}




