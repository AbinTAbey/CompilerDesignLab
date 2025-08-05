#include <string.h>
#include <ctype.h>
#include <stdio.h>

void main()
{
    FILE *f1;
    char c, str[10];
    int lineno = 1, num = 0, i = 0;

    // Ask user for a C program as input; write it to a file
    printf("\nEnter the C program\n");
    f1 = fopen("input.txt", "w");
    // Read characters until EOF and write to file
    while ((c = getchar()) != EOF)
        putc(c, f1);
    fclose(f1);

    // Open the file for reading
    f1 = fopen("input.txt", "r");
    while ((c = getc(f1)) != EOF)
    {
        // If the character is a digit, process a number
        if (isdigit(c))
        {
            num = c - 48; // Start from this digit
            c = getc(f1);
            // Continue as long as next char is a digit
            while (isdigit(c))
            {
                num = num * 10 + (c - 48);
                c = getc(f1);
            }
            printf("%d is a number \n", num);
            ungetc(c, f1); // Push non-digit back in file for next loop
        }
        // If the character is an alphabet, process an identifier/keyword
        else if (isalpha(c))
        {
            str[i++] = c;
            c = getc(f1);
            // Accept more letters, digits, '_', or '$'
            while (isdigit(c) || isalpha(c) || c == '_' || c == '$')
            {
                str[i++] = c;
                c = getc(f1);
            }
            str[i++] = '\0';
            // Check if found string is a C keyword
            if (strcmp("for", str) == 0 || strcmp("while", str) == 0 || strcmp("do", str) == 0 ||
                strcmp("int", str) == 0 || strcmp("float", str) == 0 || strcmp("char", str) == 0 ||
                strcmp("double", str) == 0 || strcmp("static", str) == 0 ||
                strcmp("switch", str) == 0 || strcmp("case", str) == 0)
                printf("%s is a keyword\n", str);
            else
                printf("%s is an identifier\n", str);

            ungetc(c, f1); // Push back non-valid char
            i = 0;         // Reset string index
        }
        // If space or tab, print newline for formatting
        else if (c == ' ' || c == '\t')
            printf("\n");
        // If new line, increment line number counter
        else if (c == '\n')
            lineno++;
        // For all other characters, treat as special symbol
        else
            printf("%c is a special symbol\n", c);
    }

    // Print total lines
    printf("Total no. of lines are: %d\n", lineno);
    fclose(f1);

    getchar();
}

