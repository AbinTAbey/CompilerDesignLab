#include <stdio.h>
#include <string.h>

#define MAX 10

char grammar[MAX][20] = {
    "E->TX",
    "X->+TX",
    "X->@",
    "T->FY",
    "Y->*FY",
    "Y->@",
    "F->(E)",
    "F->i"
};

char terminals[] = {'(', ')', '+', '*', 'i', '$'};
char nonterminals[] = {'E', 'T', 'X', 'Y', 'F'};

char FIRST[10][20];
char FOLLOW[10][20];

int n_terms = 6, n_nonterms = 5, n_productions = 8;

int is_terminal(char ch) {
    for (int i = 0; i < n_terms; i++)
        if (terminals[i] == ch)
            return 1;
    return 0;
}

int nonterm_index(char ch) {
    for (int i = 0; i < n_nonterms; i++)
        if (nonterminals[i] == ch)
            return i;
    return -1;
}

void add_to_set(char *set, char ch) {
    if (!strchr(set, ch)) {
        int len = strlen(set);
        set[len] = ch;
        set[len + 1] = '\0';
    }
}

void compute_first(char ch, char *result);

void first_of_string(const char *str, char *result) {
    // result should be an initialized string
    if (str[0] == '\0' || str[0] == '@') {
        add_to_set(result, '@');
        return;
    }

    int all_have_epsilon = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        char temp[20] = "";
        compute_first(str[i], temp);

        int has_epsilon = 0;
        for (int j = 0; temp[j]; j++) {
            if (temp[j] == '@') {
                has_epsilon = 1;
            } else {
                add_to_set(result, temp[j]);
            }
        }

        if (has_epsilon == 0) {
            all_have_epsilon = 0;
            break;
        }
    }
    if (all_have_epsilon)
        add_to_set(result, '@');
}

void compute_first(char ch, char *result) {
    if (is_terminal(ch)) {
        add_to_set(result, ch);
        return;
    }

    int idx = nonterm_index(ch);
    if (FIRST[idx][0]) {
        strcpy(result, FIRST[idx]);
        return;
    }

    char temp_result[20] = "";
    for (int i = 0; i < n_productions; i++) {
        if (grammar[i][0] == ch) {
            char *rhs = &grammar[i][3];
            char temp[20] = "";
            first_of_string(rhs, temp);

            for (int k = 0; temp[k]; k++)
                add_to_set(temp_result, temp[k]);
        }
    }
    strcpy(FIRST[idx], temp_result);
    strcpy(result, temp_result);
}

void compute_follow(char ch, char *result) {
    int idx = nonterm_index(ch);
    if (FOLLOW[idx][0]) {
        strcpy(result, FOLLOW[idx]);
        return;
    }

    char temp_result[20] = "";

    // Add $ for start symbol
    if (ch == 'E')
        add_to_set(temp_result, '$');

    for (int i = 0; i < n_productions; i++) {
        char *rhs = &grammar[i][3];
        for (int j = 0; rhs[j] != '\0'; j++) {
            if (rhs[j] == ch) {
                char temp[20] = "";
                if (rhs[j+1] != '\0') {
                    first_of_string(&rhs[j+1], temp);
                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != '@')
                            add_to_set(temp_result, temp[k]);
                    }
                }
                // If at end or following can be epsilon
                if (rhs[j+1] == '\0' || strchr(temp, '@')) {
                    // recur for LHS
                    if (grammar[i][0] != ch) { // avoid left rec
                        char t2[20] = "";
                        compute_follow(grammar[i][0], t2);
                        for (int k = 0; t2[k]; k++)
                            add_to_set(temp_result, t2[k]);
                    }
                }
            }
        }
    }
    strcpy(FOLLOW[idx], temp_result);
    strcpy(result, temp_result);
}

int main() {
    // Initialize FIRST and FOLLOW tables to empty strings
    for (int i = 0; i < n_nonterms; i++) {
        FIRST[i][0] = '\0';
        FOLLOW[i][0] = '\0';
    }

    printf("\n\tGRAMMAR\n\t--------\n");
    for (int i = 0; i < n_productions; i++)
        printf("\t%s\n", grammar[i]);

    printf("\n\tFIRST SETS\n\t-----------\n");
    for (int i = 0; i < n_nonterms; i++) {
        char result[20] = "";
        compute_first(nonterminals[i], result);
        printf("\tFIRST(%c) = { ", nonterminals[i]);
        for (int j = 0; result[j]; j++) {
            if (result[j] == '@')
                printf("ε ");
            else
                printf("%c ", result[j]);
        }
        printf("}\n");
    }

    printf("\n\tFOLLOW SETS\n\t------------\n");
    for (int i = 0; i < n_nonterms; i++) {
        char result[20] = "";
        compute_follow(nonterminals[i], result);
        printf("\tFOLLOW(%c) = { ", nonterminals[i]);
        for (int j = 0; result[j]; j++)
            printf("%c ", result[j]);
        printf("}\n");
    }

    return 0;
}

