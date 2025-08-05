#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to return precedence of operators
int pre(char t) {
    if (t == '+' || t == '-')
        return 1;
    if (t == '*' || t == '/')
        return 2;
    if (t == '^')
        return 3;
    return 0;
}

int main() {
    char in[25], post[25];      // in: input (infix), post: postfix expression
    char stack[25];             // operator stack
    char st[25][25];            // stack for three address code constructs
    char temp[5];               // for temporary variable names
    int s, p, i, j, a, b, top, r;

    printf("\n\tOUTPUT\n");
    printf("**************************\n");
    printf("Enter the expression:\n");
    scanf("%s", in);

    s = 1;                          // stack pointer for operator stack
    p = 0;                          // index for postfix expression
    stack[0] = '(';                 // push '(' onto operator stack

    int l = strlen(in);
    in[l] = ')';                    // append ')' at end of infix expression
    in[l + 1] = '\0';               // end the string

    // --- Infix to Postfix conversion ---
    for (i = 0; in[i] != '\0'; i++) {
        // If character is an operand, add to postfix output
        if (in[i] != '+' && in[i] != '-' && in[i] != '*' && in[i] != '/' && 
            in[i] != '^' && in[i] != '(' && in[i] != ')') {
            post[p++] = in[i];
        }
        // If character is '(', push to stack
        else if (in[i] == '(') {
            stack[s++] = in[i];
        }
        // If character is ')', pop stack to postfix until '(' is found
        else if (in[i] == ')') {
            while (stack[s - 1] != '(') {
                post[p++] = stack[--s];
            }
            s--; // Pop '('
        }
        // If operator, pop operators from stack with higher or equal precedence
        else {
            a = pre(in[i]);
            b = pre(stack[s - 1]);
            while (a <= b) {
                post[p++] = stack[--s];
                b = pre(stack[s - 1]);
            }
            stack[s++] = in[i];
        }
    }

    post[p] = '\0'; // terminate postfix string

    printf("\n\nThe postfix notation for the given expression: \n\n ");
    for (j = 0; post[j] != '\0'; j++)
        printf("%c", post[j]);
    printf("\n\nThree address code\n\n ");

    // --- Postfix to Three Address Code Generation ---
    top = 0;    // top for three-address code stack
    r = 1;      // temporary variable counter

    for (p = 0; post[p] != '\0'; p++) {
        // If operand, push to stack for TAC
        if (post[p] != '+' && post[p] != '-' && post[p] != '*' && 
            post[p] != '/' && post[p] != '^') {
            st[top][0] = post[p];
            st[top][1] = '\0';
            top++;
        }
        // If operator, pop two operands, form TAC, and push result
        else {
            printf("t%d=%s%c%s\n", r, st[top - 2], post[p], st[top - 1]);
            snprintf(temp, sizeof(temp), "t%d", r);
            strcpy(st[top - 2], temp); // Replace with new temp variable
            r++;
            top--; // Two popped, one pushed
        }
    }

    // Assign final result to the first character of the input
    printf("%c=%s\n", in[0], st[0]);
    return 0;
}
