#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Precedence function
int pre(char t) {
    if (t == '+' || t == '-') return 1;
    if (t == '*' || t == '/') return 2;
    if (t == '^') return 3;
    return 0;
}

int main() {
    char in[50], post[50], stack[50], st[20][20], temp[10];
    int s, p, i, len, top, tempno;
    char lhs[20];

    printf("\n\tOUTPUT\n");
    printf("**************************\n");
    printf("Enter the expression:\n");
    scanf("%s", in);

    // Split at '=' sign
    char *eq = strchr(in, '=');
    if (!eq) {
        printf("Invalid expression! No '=' found.\n");
        return 1;
    }
    // Copy LHS
    strncpy(lhs, in, eq - in);
    lhs[eq - in] = '\0';
    // RHS to work with
    char rhs[50];
    strcpy(rhs, eq + 1);

    // Infix to postfix for RHS
    len = strlen(rhs);
    rhs[len] = ')';
    rhs[len+1]='\0';
    stack[0] = '(';
    s = 1;
    p = 0;

    for (i = 0; rhs[i] != '\0'; i++) {
        if (rhs[i] != '+' && rhs[i] != '-' && rhs[i] != '*' && rhs[i] != '/' && rhs[i] != '^' && rhs[i] != '(' && rhs[i] != ')') {
            post[p++] = rhs[i];
        } else if (rhs[i] == '(') {
            stack[s++] = rhs[i];
        } else if (rhs[i] == ')') {
            while (stack[s-1] != '(') {
                post[p++] = stack[--s];
            }
            s--;
        } else { // Operator
            int a = pre(rhs[i]);
            int b = pre(stack[s-1]);
            while (a <= b) {
                post[p++] = stack[--s];
                b = pre(stack[s-1]);
            }
            stack[s++] = rhs[i];
        }
    }
    post[p] = '\0';

    printf("\n\nThe postfix notation for the given expression : \n\n");
    printf(" %s=%s\n", lhs, post);

    printf("\n Three address code \n\n");
    top = 0;
    tempno = 0;
    for (p = 0; post[p] != '\0'; p++) {
        if (post[p] != '+' && post[p] != '-' && post[p] != '*' && post[p] != '/' && post[p] != '^') {
            st[top][0] = post[p];
            st[top][1] = '\0';
            top++;
        } else {
            printf("t%d=%s%c%s\n", tempno, st[top-2], post[p], st[top-1]);
            snprintf(temp, sizeof(temp), "t%d", tempno);
            strcpy(st[top-2], temp);
            tempno++;
            top--;
        }
    }
    printf("%s=%s\n", lhs, st[0]);
    return 0;
}

