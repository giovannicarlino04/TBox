#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

FILE* openFile(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    return file;
}

void editLine(char* buffer, int currentLine) {
    char* startOfLine = buffer;
    for (int i = 0; i < currentLine; i++) {
        startOfLine = strchr(startOfLine, '\n');
        if (startOfLine == NULL) {
            printf("Error: Line %d does not exist.\n", currentLine);
            return;
        }
        startOfLine++; // Move past the newline character
    }

    char* endOfLine = strchr(startOfLine, '\n');
    if (endOfLine == NULL) {
        endOfLine = strchr(startOfLine, '\0'); // Last line in buffer
    }

    char saved[BUFFER_SIZE] = {0};
    strcpy(saved, endOfLine); // Save the rest of the buffer after the current line

    printf("Enter the new content for line %d: ", currentLine);
    scanf(" %[^\n]", startOfLine); // Read the new content including spaces

    // Calculate position to copy saved line back into buffer
    size_t savedLen = strlen(saved);
    size_t newContentLen = strlen(startOfLine);
    size_t remainingLen = strlen(startOfLine + newContentLen);
    memmove(startOfLine + newContentLen, saved, savedLen);
    buffer[newContentLen + savedLen + remainingLen] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* file = openFile(argv[1], "r");
    
    // Allocate memory for buffer dynamically to handle large files
    char* buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read file content into buffer
    size_t bytesRead = fread(buffer, sizeof(char), BUFFER_SIZE, file);
    buffer[bytesRead] = '\0'; // Null terminate buffer
    fclose(file);

    printf("Original content:\n%s\n", buffer);

    int currentLine;
    printf("Enter the line number to edit: ");
    scanf("%d", &currentLine);

    editLine(buffer, currentLine);

    file = openFile(argv[1], "w");
    fwrite(buffer, sizeof(char), strlen(buffer), file);
    fclose(file);

    printf("File updated successfully.\n");

    // Free dynamically allocated memory
    free(buffer);

    return EXIT_SUCCESS;
}
