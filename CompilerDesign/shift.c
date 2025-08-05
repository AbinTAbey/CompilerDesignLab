#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ip_sym[15], stack[15];
int ip_ptr = 0, st_ptr = -1, len;
char act[20];

void reduce();

int main() {
    printf("\n\t\tSHIFT REDUCE PARSER\n");
    printf("\nGRAMMAR\n");
    printf("E -> E+E\nE -> E/E\nE -> E*E\nE -> a\nE -> b\n");
    printf("\nEnter the input symbol: ");
    fgets(ip_sym, sizeof(ip_sym), stdin);

    // Remove trailing newline
    len = strlen(ip_sym);
    if (len > 0 && ip_sym[len-1] == '\n') ip_sym[len-1] = '\0';
    len = strlen(ip_sym);

    printf("\nstack\t\tinputsymbol\t\taction");
    printf("\n-----------------------------------------------");
    printf("\n$\t\t%s$\t\t\t--", ip_sym);

    while (ip_ptr < len) {
        // SHIFT operation: Push next input symbol to stack
        stack[++st_ptr] = ip_sym[ip_ptr];
        stack[st_ptr+1] = '\0';
        ip_sym[ip_ptr] = ' ';
        ip_ptr++;

        // Print action
        printf("\n$%s\t\t%s$\t\t\tshift", stack, ip_sym);

        // Try to reduce as much as possible after every shift
        int reduced;
        do {
            reduced = 0;
            // Reduce a/b to E
            if (stack[st_ptr] == 'a' || stack[st_ptr] == 'b') {
                stack[st_ptr] = 'E';
                printf("\n$%s\t\t%s$\t\t\tE->%c", stack, ip_sym, stack[st_ptr]);
                reduced = 1;
            }
            // Reduce E op E to E (check for all three operators)
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
    }

    // After input consumed, attempt any leftover reductions
    int reduced;
    do {
        reduced = 0;
        if (stack[st_ptr] == 'a' || stack[st_ptr] == 'b') {
            stack[st_ptr] = 'E';
            printf("\n$%s\t\t%s$\t\t\tE->%c", stack, ip_sym, stack[st_ptr]);
            reduced = 1;
        }
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

    if (strcmp(stack, "E") == 0) {
        printf("\n$%s\t\t%s$\t\t\tACCEPT\n", stack, ip_sym);
    } else {
        printf("\n$%s\t\t%s$\t\t\treject\n", stack, ip_sym);
    }

    return 0;
}

