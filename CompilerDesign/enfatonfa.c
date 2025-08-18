#include <stdio.h>
#include <stdlib.h>

struct stateNode {
    int stateId;
    struct stateNode *next;
};

void computeEpsilonClosure(int, int);
void addTransition(int, char, int);
int getAlphabetIndex(char);
void determineFinalStates(void);
void combineClosure(int);
void displayEpsilonClosure(int);

static int closureSet[20], numStates, numAlphabets, srcState, numTransitions, numFinalStates, startState, finalStates[20], currentState, row, transitionBuffer[20];
char alphabet[20];
static int epsilonClosure[20][20] = {0};
struct stateNode *transitions[20][20] = {NULL};

int main() {
    int i, j, k, m, t, n;
    struct stateNode *tempNode;

    printf("Enter the number of alphabets:\n");
    scanf("%d", &numAlphabets);
    getchar();

    printf("NOTE: Use letter 'e' for epsilon (if present). Epsilon must be the last character.\n");

    printf("\nEnter the alphabets:\n");
    for (i = 0; i < numAlphabets; i++) {
        alphabet[i] = getchar();
        getchar();
    }

    printf("Enter the number of states:\n");
    scanf("%d", &numStates);

    printf("Enter the start state:\n");
    scanf("%d", &startState);

    printf("Enter the number of final states:\n");
    scanf("%d", &numFinalStates);

    printf("Enter the final states:\n");
    for (i = 0; i < numFinalStates; i++) {
        scanf("%d", &finalStates[i]);
    }

    printf("Enter the number of transitions:\n");
    scanf("%d", &numTransitions);

    printf("NOTE: Transitions are entered as: qno alphabet qno\n");

    for (i = 0; i < numTransitions; i++) {
        scanf("%d %c%d", &srcState, &currentState, &row);
        addTransition(srcState, currentState, row);
    }

    printf("\n");

    // Calculate epsilon closure for each state
    for (i = 1; i <= numStates; i++) {
        currentState = 0;
        for (j = 0; j < 20; j++) {
            transitionBuffer[j] = 0;
            epsilonClosure[i][j] = 0;
        }
        computeEpsilonClosure(i, i);
    }

    printf("Equivalent NFA without epsilon\n");
    printf("-----------------------------------\n");

    printf("Start state: ");
    displayEpsilonClosure(startState);

    printf("\nAlphabets: ");
    for (i = 0; i < numAlphabets; i++) {
        printf("%c ", alphabet[i]);
    }

    printf("\nStates: ");
    for (i = 1; i <= numStates; i++) {
        displayEpsilonClosure(i);
    }

    printf("\nTransitions are:\n");
    for (i = 1; i <= numStates; i++) {
        for (j = 0; j < numAlphabets - 1; j++) {
            for (m = 1; m <= numStates; m++) {
                closureSet[m] = 0;
            }

            for (k = 0; epsilonClosure[i][k] != 0; k++) {
                t = epsilonClosure[i][k];
                tempNode = transitions[t][j];
                while (tempNode != NULL) {
                    combineClosure(tempNode->stateId);
                    tempNode = tempNode->next;
                }
            }

            printf("\n");
            displayEpsilonClosure(i);
            printf("%c\t", alphabet[j]);
            printf("{");

            for (n = 1; n <= numStates; n++) {
                if (closureSet[n] != 0) {
                    printf("q%d,", n);
                }
            }
            printf("}");
        }
    }

    printf("\nFinal states: ");
    determineFinalStates();

    return 0;
}

void computeEpsilonClosure(int state, int currentClosure) {
    struct stateNode *tempNode;
    int i;

    // If this state is already included in the closure, return
    if (transitionBuffer[state]) return;

    // Add state to the epsilon closure of currentClosure
    epsilonClosure[currentClosure][currentState++] = state;
    transitionBuffer[state] = 1;

    // If the state has an epsilon transition, recursively compute closure
    if (alphabet[numAlphabets - 1] == 'e' && transitions[state][numAlphabets - 1] != NULL) {
        tempNode = transitions[state][numAlphabets - 1];

        while (tempNode != NULL) {
            computeEpsilonClosure(tempNode->stateId, currentClosure);
            tempNode = tempNode->next;
        }
    }
}

void addTransition(int fromState, char symbol, int toState) {
    int symbolIndex;
    struct stateNode *newNode;

    symbolIndex = getAlphabetIndex(symbol);
    if (symbolIndex == 999) {
        printf("Error: Invalid alphabet.\n");
        exit(0);
    }

    newNode = (struct stateNode *)malloc(sizeof(struct stateNode));
    newNode->stateId = toState;
    newNode->next = transitions[fromState][symbolIndex];
    transitions[fromState][symbolIndex] = newNode;
}

int getAlphabetIndex(char symbol) {
    int i;
    for (i = 0; i < numAlphabets; i++) {
        if (alphabet[i] == symbol) {
            return i;
        }
    }
    return 999;  // Return an invalid index if symbol not found
}

void combineClosure(int stateId) {
    int i = 0, state;
    while (epsilonClosure[stateId][i] != 0) {
        state = epsilonClosure[stateId][i];
        closureSet[state] = 1;
        i++;
    }
}

void determineFinalStates() {
    int i, j, k;
    for (i = 0; i < numFinalStates; i++) {
        for (j = 1; j <= numStates; j++) {
            for (k = 0; epsilonClosure[j][k] != 0; k++) {
                if (epsilonClosure[j][k] == finalStates[i]) {
                    displayEpsilonClosure(j);
                }
            }
        }
    }
}

void displayEpsilonClosure(int state) {
    int i;
    printf("{");
    for (i = 0; epsilonClosure[state][i] != 0; i++) {
        printf("q%d,", epsilonClosure[state][i]);
    }
    printf("}\t");
}

/*
Example Output:

Enter the number of alphabets:
3
NOTE: Use letter 'e' for epsilon (if present). Epsilon must be the last character.

Enter the alphabets:
a
b
e
Enter the number of states:
5
Enter the start state:
1
Enter the number of final states:
1
Enter the final states:
3
Enter the number of transitions:
7
NOTE: Transitions are entered as: qno alphabet qno
1 a 2
1 e 3
2 a 1
3 b 4
4 b 3
3 a 5
5 b 3

Equivalent NFA without epsilon
-----------------------------------
Start state: {q1,}
Alphabets: a b e 
States: {q1,} {q2,} {q3,} {q4,} {q5,} 
Transitions are:
{q1,} a {q2,q5,}
{q1,} b {q4,}
{q2,} a {q1,q3,}
{q2,} b {}
{q3,} a {q5,}
{q3,} b {q4,}
{q4,} a {}
{q4,} b {q3,}
{q5,} a {}
{q5,} b {q3,}

Final states: {q1,} {q3,}
*/
