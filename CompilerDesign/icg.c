#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to return precedence of operators
int pre(char t) {
    if (t == '+' || t == '-') return 1;
    if (t == '*' || t == '/') return 2;
    if (t == '^') return 3;
    return 0;
}

int main() {
    // Input and temporary buffers
    char in[50], post[50], stack[50], st[20][20], temp[10];
    int s, p, i, len, top, tempno;
    char lhs[20];

    printf("\n\tOUTPUT\n");
    printf("**************************\n");
    printf("Enter the expression:\n");
    scanf("%s", in);

    // Split input at '=' sign (for assignments)
    char *eq = strchr(in, '=');
    if (!eq) {
        printf("Invalid expression! No '=' found.\n");
        return 1;
    }
    // Extract left-hand side
    strncpy(lhs, in, eq - in);
    lhs[eq - in] = '\0';
    // Right-hand side to process
    char rhs[50];
    strcpy(rhs, eq + 1);

    // Infix to postfix conversion for RHS
    len = strlen(rhs);
    rhs[len] = ')';      // Append closing parenthesis for processing
    rhs[len+1] = '\0';
    stack[0] = '(';      // Start with '(' on stack
    s = 1;               // Stack pointer index
    p = 0;               // Postfix output index

    // Conversion logic
    for (i = 0; rhs[i] != '\0'; i++) {
        // Operand: direct to output
        if (rhs[i] != '+' && rhs[i] != '-' && rhs[i] != '*' && rhs[i] != '/' && rhs[i] != '^' && rhs[i] != '(' && rhs[i] != ')') {
            post[p++] = rhs[i];
        }
        // Left parenthesis: push to stack
        else if (rhs[i] == '(') {
            stack[s++] = rhs[i];
        }
        // Right parenthesis: pop till '('
        else if (rhs[i] == ')') {
            while (stack[s-1] != '(') {
                post[p++] = stack[--s];
            }
            s--; // Pop '('
        }
        // Operator: handle precedence and associativity
        else {
            int a = pre(rhs[i]);
            int b = pre(stack[s-1]);
            while (a <= b) {
                post[p++] = stack[--s];
                b = pre(stack[s-1]);
            }
            stack[s++] = rhs[i];
        }
    }
    post[p] = '\0'; // Null-terminate postfix

    // Print postfix result
    printf("\n\nThe postfix notation for the given expression : \n\n");
    printf(" %s=%s\n", lhs, post);

    // Three address code generation from postfix
    printf("\n Three address code \n\n");
    top = 0;     // Stack top for three-address operands
    tempno = 0;  // Temp variable counter

    for (p = 0; post[p] != '\0'; p++) {
        // Operand: push as string
        if (post[p] != '+' && post[p] != '-' && post[p] != '*' && post[p] != '/' && post[p] != '^') {
            st[top][0] = post[p];
            st[top][1] = '\0';
            top++;
        }
        // Operator: pop two, create temp assignment, push result
        else {
            printf("t%d=%s%c%s\n", tempno, st[top-2], post[p], st[top-1]);
            snprintf(temp, sizeof(temp), "t%d", tempno);
            strcpy(st[top-2], temp);
            tempno++;
            top--;
        }
    }
    // Assign final temp or result to lhs variable
    printf("%s=%s\n", lhs, st[0]);
    return 0;
}
