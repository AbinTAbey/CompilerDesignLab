#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure to store expression details
struct expr {
    char op[2], op1[10], op2[10], res[10];
    int flag;
} arr[10];

int n;

void input();
void constant();
void output();
void change(int p, char* res);

int main() {
    input();
    constant();
    output();
    return 0;
}

void input() {
    int i;
    printf("\n\nEnter the maximum number of expressions: ");
    scanf("%d", &n);
    printf("\nEnter the input expressions (op op1 op2 res): \n");
    for (i = 0; i < n; i++) {
        scanf("%s %s %s %s", arr[i].op, arr[i].op1, arr[i].op2, arr[i].res);
        arr[i].flag = 0;
    }
}

void constant() {
    int i;
    int op1, op2, res;
    char res1[10];
    char op;

    for (i = 0; i < n; i++) {
        // Check if op1 is a digit (immediate number)
        int op1_is_digit = isdigit(arr[i].op1[0]) || (arr[i].op1[0] == '-' && isdigit(arr[i].op1[1]));
        int op2_is_digit = isdigit(arr[i].op2[0]) || (arr[i].op2[0] == '-' && isdigit(arr[i].op2[1]));

        if ((op1_is_digit && op2_is_digit) || strcmp(arr[i].op, "=") == 0) {
            sscanf(arr[i].op1, "%d", &op1);

            if (strcmp(arr[i].op, "=") != 0) {
                sscanf(arr[i].op2, "%d", &op2);
            }

            op = arr[i].op[0];

            switch (op) {
                case '+': res = op1 + op2; break;
                case '-': res = op1 - op2; break;
                case '*': res = op1 * op2; break;
                case '/':
                    if (op2 == 0) {
                        printf("\nError: Division by zero in expression %d\n", i + 1);
                        continue;
                    }
                    res = op1 / op2;
                    break;
                case '=': res = op1; break;
                default:
                    // unsupported operator
                    continue;
            }
            sprintf(res1, "%d", res);
            arr[i].flag = 1;  // Mark expression as foldable to a constant
            change(i, res1);
        }
    }
}

void change(int p, char* res) {
    int i;
    for (i = p + 1; i < n; i++) {
        if (strcmp(arr[p].res, arr[i].op1) == 0)
            strcpy(arr[i].op1, res);
        if (strcmp(arr[p].res, arr[i].op2) == 0)
            strcpy(arr[i].op2, res);
    }
}

void output() {
    int i;
    printf("\nOptimized code is:\n");
    for (i = 0; i < n; i++) {
        if (!arr[i].flag) {
            printf("%s %s %s %s\n", arr[i].op, arr[i].op1, arr[i].op2, arr[i].res);
        }
    }
}

