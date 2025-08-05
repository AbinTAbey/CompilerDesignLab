#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Precedence
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
    char in[25], post[25], stack[25], st[25][25], temp[5];
    int s, p, i, j, a, b, top, r;

    printf("\n\tOUTPUT\n");
    printf("**************************\n");
    printf("Enter the expression:\n");
    scanf("%s", in);

    s = 1;
    p = 0;
    stack[0] = '(';
    int l = strlen(in);
    in[l] = ')';
    in[l+1] = '\0';

    // Infix to Postfix conversion
    for (i = 0; in[i] != '\0'; i++) {
        if (in[i] != '+' && in[i] != '-' && in[i] != '*' && in[i] != '/' && in[i] != '^' && in[i] != '(' && in[i] != ')') {
            post[p++] = in[i];
        } else if (in[i] == '(') {
            stack[s++] = in[i];
        } else if (in[i] == ')') {
            while (stack[s-1] != '(') {
                post[p++] = stack[--s];
            }
            s--; // Pop '('
        } else { // Operator
            a = pre(in[i]);
            b = pre(stack[s-1]);
            while (a <= b) {
                post[p++] = stack[--s];
                b = pre(stack[s-1]);
            }
            stack[s++] = in[i];
        }
    }

    post[p] = '\0';

    printf("\n\nThe postfix notation for the given expression : \n\n ");
    for (j = 0; post[j] != '\0'; j++)
        printf("%c", post[j]);
    printf("\n\n Three address code \n\n ");

    top = 0;
    r = 1;
    for (p = 0; post[p] != '\0'; p++) {
        if (post[p] != '+' && post[p] != '-' && post[p] != '*' && post[p] != '/' && post[p] != '^') {
            st[top][0] = post[p];
            st[top][1] = '\0';
            top++;
        } else { // Operator
            printf("t%d=%s%c%s\n", r, st[top-2], post[p], st[top-1]);
            snprintf(temp, sizeof(temp), "t%d", r);
            strcpy(st[top-2], temp);
            r++;
            top--; // Two popped, one pushed
        }
    }
    printf("%c=%s\n", in[0], st[0]);
    return 0;
}

