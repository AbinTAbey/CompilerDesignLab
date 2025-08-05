#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Input symbols buffer and stack
char ip_sym[15], stack[15];
// Input pointer and stack pointer
int ip_ptr = 0, st_ptr = -1, len;
char act[20];

void reduce();

int main() {
    // Print introductory message and the grammar used by the parser
    printf("\n\t\tSHIFT REDUCE PARSER\n");
    printf("\nGRAMMAR\n");
    printf("E -> E+E\nE -> E/E\nE -> E*E\nE -> a\nE -> b\n");

    printf("\nEnter the input symbol: ");
    // Read input symbols (infix expression: e.g., a+b, a*b, etc.)
    fgets(ip_sym, sizeof(ip_sym), stdin);

    // Remove trailing newline character from input, if present
    len = strlen(ip_sym);
    if (len > 0 && ip_sym[len-1] == '\n')
        ip_sym[len-1] = '\0';
    len = strlen(ip_sym);

    // Print header for trace output
    printf("\nstack\t\tinputsymbol\t\taction");
    printf("\n-----------------------------------------------");
    // Print initial stack and input
    printf("\n$\t\t%s$\t\t\t--", ip_sym);

    // Shift and reduce operations for each input symbol
    while (ip_ptr < len) {
        // SHIFT: Push next input symbol to the stack
        stack[++st_ptr] = ip_sym[ip_ptr];
        stack[st_ptr+1] = '\0';    // Null-terminate stack string

        ip_sym[ip_ptr] = ' ';      // Mark symbol as consumed in input
        ip_ptr++;                  // Move input pointer to the right

        // Print current stack, input, and shift action
        printf("\n$%s\t\t%s$\t\t\tshift", stack, ip_sym);

        // Try to reduce the stack as much as possible after each shift
        int reduced;
        do {
            reduced = 0;

            // REDUCE: Reduce 'a' or 'b' to 'E'
            if (stack[st_ptr] == 'a' || stack[st_ptr] == 'b') {
                stack[st_ptr] = 'E'; // Replace 'a' or 'b' with 'E'
                printf("\n$%s\t\t%s$\t\t\tE->%c", stack, ip_sym, stack[st_ptr]);
                reduced = 1;
            }
            // REDUCE: Reduce E+E / E*E / E/E to E
            else if (st_ptr >= 2 &&
                     stack[st_ptr-2] == 'E' &&
                    (stack[st_ptr-1] == '+' || stack[st_ptr-1] == '*' || stack[st_ptr-1] == '/') &&
                     stack[st_ptr] == 'E') {
                char op = stack[st_ptr-1];     // Store operator for action print
                stack[st_ptr-2] = 'E';         // Replace pattern with 'E'
                st_ptr -= 2;                   // Adjust stack pointer
                stack[st_ptr+1] = '\0';        // Null-terminate stack string
                printf("\n$%s\t\t%s$\t\t\tE->E%cE", stack, ip_sym, op);
                reduced = 1;
            }
        } while (reduced);
    }

    // After consuming all input, attempt any leftover reductions
    int reduced;
    do {
        reduced = 0;
        // REDUCE: Final 'a' or 'b' at stack top
        if (stack[st_ptr] == 'a' || stack[st_ptr] == 'b') {
            stack[st_ptr] = 'E';
            printf("\n$%s\t\t%s$\t\t\tE->%c", stack, ip_sym, stack[st_ptr]);
            reduced = 1;
        }
        // REDUCE: Final E op E patterns
        else if (st_ptr >= 2 && stack[st_ptr-2] == 'E' &&
                 (stack[st_ptr-1] == '+' || stack[st_ptr-1] == '*' || stack[st_ptr-1] == '/') &&
                 stack[st_ptr] == 'E') {
            char op = stack[st_ptr-1];
            stack[st_ptr-2] = 'E';
            st_ptr -= 2;
            stack[st_ptr+1] = '\0';
            printf("\n$%s\t\t%s$\t\t\tE->E%cE", stack, ip_sym, op);
            reduced = 1;
        }
    } while (reduced);

    // ACCEPT if the entire stack reduces to just 'E'
    if (strcmp(stack, "E") == 0) {
        printf("\n$%s\t\t%s$\t\t\tACCEPT\n", stack, ip_sym);
    } else {
        printf("\n$%s\t\t%s$\t\t\treject\n", stack, ip_sym);
    }

    return 0;
}
