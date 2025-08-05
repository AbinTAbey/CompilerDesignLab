#include <stdio.h>
#include <string.h>

#define MAX 10

// Grammar productions representation
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

// List of all terminals and nonterminals in the grammar
char terminals[] = {'(', ')', '+', '*', 'i', '$'};
char nonterminals[] = {'E', 'T', 'X', 'Y', 'F'};

// Arrays to store FIRST and FOLLOW sets per nonterminal
char FIRST[10][20];
char FOLLOW[10][20];

int n_terms = 6, n_nonterms = 5, n_productions = 8;

// Check if a character is a terminal symbol
int is_terminal(char ch) {
    for (int i = 0; i < n_terms; i++)
        if (terminals[i] == ch)
            return 1;
    return 0;
}

// Get the index of a nonterminal in the nonterminals array
int nonterm_index(char ch) {
    for (int i = 0; i < n_nonterms; i++)
        if (nonterminals[i] == ch)
            return i;
    return -1;
}

// Add a character to a set if it's not already present
void add_to_set(char *set, char ch) {
    if (!strchr(set, ch)) {
        int len = strlen(set);
        set[len] = ch;
        set[len + 1] = '\0';
    }
}

// Forward declaration
void compute_first(char ch, char *result);

// Compute FIRST for a sequence of symbols (a grammar right-hand side)
void first_of_string(const char *str, char *result) {
    // If epsilon or end of string, just add epsilon
    if (str[0] == '\0' || str[0] == '@') {
        add_to_set(result, '@');
        return;
    }

    int all_have_epsilon = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        char temp[20] = "";
        compute_first(str[i], temp);

        int has_epsilon = 0;
        // Add everything but epsilon from current symbol's FIRST to result
        for (int j = 0; temp[j]; j++) {
            if (temp[j] == '@') {
                has_epsilon = 1;
            } else {
                add_to_set(result, temp[j]);
            }
        }

        // If current symbol does not have epsilon, stop looking further
        if (has_epsilon == 0) {
            all_have_epsilon = 0;
            break;
        }
    }
    // If all symbols had epsilon, add epsilon to result
    if (all_have_epsilon)
        add_to_set(result, '@');
}

// Compute FIRST set for a nonterminal or terminal symbol
void compute_first(char ch, char *result) {
    // Terminal's FIRST is itself
    if (is_terminal(ch)) {
        add_to_set(result, ch);
        return;
    }

    int idx = nonterm_index(ch);
    // Use cached value if already computed
    if (FIRST[idx][0]) {
        strcpy(result, FIRST[idx]);
        return;
    }

    char temp_result[20] = "";
    // For each production for this nonterminal
    for (int i = 0; i < n_productions; i++) {
        if (grammar[i][0] == ch) {
            char *rhs = &grammar[i][3]; // Get the right side of production
            char temp[20] = "";
            first_of_string(rhs, temp);

            // Add result from sequence's FIRST to temp_result
            for (int k = 0; temp[k]; k++)
                add_to_set(temp_result, temp[k]);
        }
    }
    // Store in global table and copy to result
    strcpy(FIRST[idx], temp_result);
    strcpy(result, temp_result);
}

// Compute FOLLOW set for nonterminal symbol ch
void compute_follow(char ch, char *result) {
    int idx = nonterm_index(ch);

    // Use cached value if already computed
    if (FOLLOW[idx][0]) {
        strcpy(result, FOLLOW[idx]);
        return;
    }

    char temp_result[20] = "";

    // Start symbol gets end marker
    if (ch == 'E')
        add_to_set(temp_result, '$');

    // Look in all productions for occurrences of ch
    for (int i = 0; i < n_productions; i++) {
        char *rhs = &grammar[i][3];
        for (int j = 0; rhs[j] != '\0'; j++) {
            if (rhs[j] == ch) {
                char temp[20] = "";
                // If ch is not at the end, add FIRST of next symbol (except epsilon)
                if (rhs[j+1] != '\0') {
                    first_of_string(&rhs[j+1], temp);
                    for (int k = 0; temp[k]; k++) {
                        if (temp[k] != '@')
                            add_to_set(temp_result, temp[k]);
                    }
                }
                // If at end or following FIRST can be epsilon, add FOLLOW of LHS
                if (rhs[j+1] == '\0' || strchr(temp, '@')) {
                    // Avoid self-recursion
                    if (grammar[i][0] != ch) {
                        char t2[20] = "";
                        compute_follow(grammar[i][0], t2);
                        for (int k = 0; t2[k]; k++)
                            add_to_set(temp_result, t2[k]);
                    }
                }
            }
        }
    }

    // Save to global table and copy to result
    strcpy(FOLLOW[idx], temp_result);
    strcpy(result, temp_result);
}

int main() {
    // Initialize FIRST and FOLLOW tables
    for (int i = 0; i < n_nonterms; i++) {
        FIRST[i][0] = '\0';
        FOLLOW[i][0] = '\0';
    }

    // Print grammar
    printf("\n\tGRAMMAR\n\t--------\n");
    for (int i = 0; i < n_productions; i++)
        printf("\t%s\n", grammar[i]);

    // Compute and print FIRST sets for each nonterminal
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

    // Compute and print FOLLOW sets for each nonterminal
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
