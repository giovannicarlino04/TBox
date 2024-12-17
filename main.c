#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Function to delete a specific line
int deleteLine(char **buffer, int *numLines, int line) {
    if (buffer == NULL || numLines == NULL || line < 0 || line >= *numLines) {
        printf("Invalid line number.\n");
        return -1;
    }

    // Free the memory of the line to be deleted
    free(buffer[line]);

    // Shift lines up to fill the gap
    for (int i = line; i < *numLines - 1; i++) {
        buffer[i] = buffer[i + 1];
    }

    // Nullify the last line
    buffer[*numLines - 1] = NULL;

    // Reduce the count of lines
    (*numLines)--;
    printf("Line %d deleted successfully.\n", line);
    return 0;
}

// Function to edit a specific line
void editLine(char **buffer, int numLines, int line) {
    if (line < 0 || line >= numLines) {
        printf("Invalid line number.\n");
        return;
    }

    printf("Enter the new content for line %d: ", line);
    char newContent[BUFFER_SIZE];
    fgets(newContent, BUFFER_SIZE, stdin);
    newContent[strcspn(newContent, "\n")] = '\0'; // Remove trailing newline

    // Replace the old content with the new one
    free(buffer[line]);
    buffer[line] = strdup(newContent);
    printf("Line %d edited successfully.\n", line);
}

// Function to display the content of the buffer
void displayContent(char **buffer, int numLines) {
    printf("\n--- File Content ---\n");
    for (int i = 0; i < numLines; i++) {
        printf("%d: %s\n", i, buffer[i]);
    }
    printf("--------------------\n");
}

// Function to add a new line
void addLine(char ***buffer, int *numLines, const char *newContent) {
    // Reallocate memory to hold one more line
    *buffer = realloc(*buffer, (*numLines + 1) * sizeof(char *));
    if (*buffer == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Add the new line
    (*buffer)[*numLines] = strdup(newContent);
    (*numLines)++;
}

// Function to read a file into a line buffer
char **readFile(const char *filename, int *numLines) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char **buffer = NULL;
    char line[BUFFER_SIZE];
    *numLines = 0;

    while (fgets(line, BUFFER_SIZE, file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline
        addLine(&buffer, numLines, line);
    }

    fclose(file);
    return buffer;
}

// Function to save the buffer back to the file
void saveToFile(const char *filename, char **buffer, int numLines) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numLines; i++) {
        fprintf(file, "%s\n", buffer[i]);
    }

    fclose(file);
    printf("Changes saved successfully.\n");
}

// Main program
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read the file into the line buffer
    int numLines = 0;
    char **buffer = readFile(argv[1], &numLines);

    printf("Welcome to TBOX! The Simple Text Editor!\n");
    int choice;
    do {
        displayContent(buffer, numLines);

        printf("\nChoose an option:\n");
        printf("1. Edit a line\n");
        printf("2. Add a new line\n");
        printf("3. Delete a line\n");
        printf("4. Save and exit\n");
        printf("5. Exit without saving\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left by scanf

        int line;
        switch (choice) {
            case 1:
                printf("Enter the line number to edit: ");
                scanf("%d", &line);
                getchar();
                editLine(buffer, numLines, line);
                break;
            case 2:
                printf("Enter the new content to add: ");
                char newLine[BUFFER_SIZE];
                fgets(newLine, BUFFER_SIZE, stdin);
                newLine[strcspn(newLine, "\n")] = '\0';
                addLine(&buffer, &numLines, newLine);
                break;
            case 3:
                printf("Enter the line number to delete: ");
                scanf("%d", &line);
                getchar();
                deleteLine(buffer, &numLines, line);
                break;
            case 4:
                saveToFile(argv[1], buffer, numLines);
                choice = 5; // Exit after saving
                break;
            case 5:
                printf("Exiting without saving...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
                break;
        }
    } while (choice != 5);

    // Free the buffer
    for (int i = 0; i < numLines; i++) {
        free(buffer[i]);
    }
    free(buffer);

    return EXIT_SUCCESS;
}
