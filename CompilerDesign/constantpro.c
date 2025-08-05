#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure to store each expression/quadruple
struct expr {
    char op[2];    // Operator (+, -, *, /, =)
    char op1[10];  // Operand 1
    char op2[10];  // Operand 2
    char res[10];  // Result variable
    int flag;      // Marks if this expr is foldable/optimizable (1 = yes)
} arr[10];

int n;

// Function prototypes
void input();
void constant();
void output();
void change(int p, char* res);

int main() {
    input();     // Read all expressions
    constant();  // Perform constant folding
    output();    // Output optimized code
    return 0;
}

// Read expressions as op op1 op2 res (one quadruple per line)
void input() {
    int i;
    printf("\n\nEnter the maximum number of expressions: ");
    scanf("%d", &n);
    printf("\nEnter the input expressions (op op1 op2 res): \n");
    for (i = 0; i < n; i++) {
        scanf("%s %s %s %s", arr[i].op, arr[i].op1, arr[i].op2, arr[i].res);
        arr[i].flag = 0; // Initially, all not optimized
    }
}

// Check for constant expressions, evaluate them, and propagate constant result onward
void constant() {
    int i;
    int op1, op2, res;
    char res1[10];
    char op;

    for (i = 0; i < n; i++) {
        // Check if operands are both numbers (consider negatives)
        int op1_is_digit = isdigit(arr[i].op1[0]) || (arr[i].op1[0] == '-' && isdigit(arr[i].op1[1]));
        int op2_is_digit = isdigit(arr[i].op2[0]) || (arr[i].op2[0] == '-' && isdigit(arr[i].op2[1]));

        // Constant fold if both operands are numeric OR if it's an assignment (=)
        if ((op1_is_digit && op2_is_digit) || strcmp(arr[i].op, "=") == 0) {
            sscanf(arr[i].op1, "%d", &op1);

            if (strcmp(arr[i].op, "=") != 0) {
                sscanf(arr[i].op2, "%d", &op2);
            }

            op = arr[i].op[0];

            // Perform constant folding
            switch (op) {
                case '+': res = op1 + op2; break;
                case '-': res = op1 - op2; break;
                case '*': res = op1 * op2; break;
                case '/':
                    if (op2 == 0) {
                        printf("\nError: Division by zero in expression %d\n", i + 1);
                        continue; // Skip this fold
                    }
                    res = op1 / op2;
                    break;
                case '=': res = op1; break; // Assignment, single operand
                default:
                    // If the operator is unsupported, do nothing
                    continue;
            }
            sprintf(res1, "%d", res); // Convert result to string
            arr[i].flag = 1;          // Mark this expression as optimized
            change(i, res1);          // Replace uses of this result with const value
        }
    }
}

// Propagate constant value to all subsequent expressions
void change(int p, char* res) {
    int i;
    for (i = p + 1; i < n; i++) {
        // If result of folded expr is later used as op1, replace it
        if (strcmp(arr[p].res, arr[i].op1) == 0)
            strcpy(arr[i].op1, res);
        // If used as op2, replace
        if (strcmp(arr[p].res, arr[i].op2) == 0)
            strcpy(arr[i].op2, res);
    }
}

// Output expressions that were not marked as constant-folded
void output() {
    int i;
    printf("\nOptimized code is:\n");
    for (i = 0; i < n; i++) {
        if (!arr[i].flag) {
            printf("%s %s %s %s\n", arr[i].op, arr[i].op1, arr[i].op2, arr[i].res);
        }
    }
}
