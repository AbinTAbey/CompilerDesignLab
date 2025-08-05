#include <stdio.h>
#include <string.h>
#define epsilon 238  // Unique value to visually represent epsilon in outputs

int flag = 0, len, pt = 0;
char st[25];

// Function prototypes for grammar nonterminals
void E();
void T();
void F();
void EPRIME();
void TPRIME();
void ADVANCE();

int main() {
    // Print parser title and grammar
    printf("\nRecursive Descent parser\n");
    printf("\nE -> T E'\nE' -> + T E' /%c\nT -> F T'\nT' -> * F T' /%c\nF -> (E)/a\n", epsilon, epsilon);

    // Read input string
    printf("enter the string :");
    scanf("%s", st);
    len = strlen(st);

    // Begin parsing from start symbol
    E();

    // String is accepted only if fully consumed and no error set
    if ((flag == 0) && (len == pt))
        printf("\n\nstring is accepted\n\n");
    else
        printf("\nstring rejected\n");

    return 0;
}

// E -> T E'
void E() {
    T();
    EPRIME();
}

// T -> F T'
void T() {
    F();
    TPRIME();
}

// E' -> + T E' | epsilon
void EPRIME() {
    if (st[pt] == '+') {
        ADVANCE();
        T();
        EPRIME();
    }
    // If not '+', implicitly processes epsilon (do nothing)
}

// T' -> * F T' | epsilon
void TPRIME() {
    if (st[pt] == '*') {
        ADVANCE();
        F();
        TPRIME();
    }
    // If not '*', implicitly processes epsilon (do nothing)
}

// F -> (E) | a
void F() {
    if (st[pt] == '(') {
        ADVANCE();
        E();
        if (st[pt] == ')')
            ADVANCE();
        else
            flag = 1;   // Error, no matching ')'
    }
    else if (st[pt] == 'a') {
        ADVANCE();
    }
    else {
        flag = 1;       // Error: not '(' or 'a'
    }
}

// Move input pointer to next symbol
void ADVANCE() {
    pt++;
}
/*
output
Recursive Descent parser

E ->TE'
E' ->+TE' /�
T ->FT'
T' ->*FT'/�
F -> (E)/a
enter the string :abc

string rejected
Recursive Descent parser

E ->TE'
E' ->+TE' /�
T ->FT'
T' ->*FT'/�
F -> (E)/a
enter the string :a+a

*/
