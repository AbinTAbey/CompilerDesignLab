#include <stdio.h>
#include <string.h>

// Arrays to store results and states
char result[20][20], copy[3], states[20][20];

// Adds a state to the result array at the given index
void add_state(char a[3], int i) {
    strcpy(result[i], a);
}

// Displays the epsilon closure for a given state
void display(int n) {
    int k = 0;
    printf("Epsilon closure of %s = {", copy);
    while (k < n) {
        printf("%s", result[k]);
        k++;
    }
    printf("}\n\n\n");
}

int main() {
    FILE *INPUT;
    INPUT = fopen("input.dat", "r");   // Open the transitions file

    char state[3];
    int end, i = 0, n, k = 0;
    char state1[3], input[3], state2[3];

    // Input: number of states and the states themselves
    printf("Enter the no: of states ");
    scanf("%d", &n);
    printf("Enter the states ");
    for (k = 0; k < 3; k++) {
        scanf("%s", states[k]);
    }

    // For each state, compute its epsilon closure
    for (k = 0; k < n; k++) {
        i = 0;
        strcpy(state, states[k]);
        strcpy(copy, state);
        add_state(state, i++);

        // Read transitions and find all states reachable by epsilon transitions
        while (1) {
            end = fscanf(INPUT, "%s%s%s", state1, input, state2);
            if (end == EOF) {
                break; // End of file
            }
            // If current state matches and input is epsilon ('e')
            if (strcmp(state, state1) == 0) {
                if (strcmp(input, "e") == 0) {
                    add_state(state2, i++);
                    strcpy(state, state2);
                }
            }
        }
        display(i); // Print the closure for this state
        rewind(INPUT); // Reset file pointer to beginning for next state
    }
    return 0;
}
