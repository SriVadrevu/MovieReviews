// Sri Vadrevu
// Project 2

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

typedef struct NODE * TREE;
struct NODE {
    char element;
    TREE leftChild, rightChild;
};

// _____                 _   _               ____            _                 _   _
//|  ___|   _ _ __   ___| |_(_) ___  _ __   |  _ \  ___  ___| | __ _ _ __ __ _| |_(_) ___  _ __  ___
//| |_ | | | | '_ \ / __| __| |/ _ \| '_ \  | | | |/ _ \/ __| |/ _` | '__/ _` | __| |/ _ \| '_ \/ __|
//|  _|| |_| | | | | (__| |_| | (_) | | | | | |_| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_| |____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
                                                                         

// ------ARGUMENTS-----
// Takes in two char pointers, both char*str and char toStore[] are of the same type
// This works since char arrays can essentially be treated as strings
// Since we are passing by reference in the function call, the function can be void
// -----HOW IT WORKS----
// We are essentially filling array toStore (given starting address of the array)
// with char values of the string (given also by a starting address of the string)
// We fill up the char array with char values until we reach end of the string
// We predetermined the length of the string prior, so we will not overflow the array.
void readString(char *str, char toStore[]);


// ------ARGUMENTS-----
// Takes starting address of string
// -----HOW IT WORKS----
// Prints string that has been read in until pointer is NULL
void printString(char *str);


// ------ARGUMENTS-----
// Takes starting address of char array that contains operands and operators of postfix
// expression.  Also takes length of this array to create an array stack.
// -----HOW IT WORKS----
// Here we push operands into the stack (of type int), and pop out two operands when we reach an operator
// This happens as we traverse through the char array.
int solvePostFix(char expression[], int length);


// ------ARGUMENTS-----
// Takes starting address of char array that contains operands and operators of postfix
// expression.  Also takes length of this array to create an array stack.
// -----HOW IT WORKS----
// We create a stack of type TREE that holds nodes that holds operators.  When we reach
// an integer we create a leaf node.  By pushing nodes and popping nodes from the stack
// we can traverse through the tree and create it.
TREE createTree(char expression[], int length);


// ------ARGUMENTS-----
// Takes pointer to the parent struct
// -----HOW IT WORKS----
// Recursive function call, prints far left-child and then goes up.
// Then prints right-childs.
// This code is a modified form of what was given in lecture.  Same form as printing
// a binary tree.
void printInfix(TREE T);


// ------ARGUMENTS-----
// Takes pointer to the parent struct
// -----HOW IT WORKS----
// Recursive function call, prints left-childs starting from the parent
// and then prints right-childs.
// This code is a modified form of what was given in lecture.  Same form as printing
// a binary tree.
void printPrefix(TREE T);


// __  __       _          ____          _
//|  \/  | __ _(_)_ __    / ___|___   __| | ___
//| |\/| |/ _` | | '_ \  | |   / _ \ / _` |/ _ \
//| |  | | (_| | | | | | | |__| (_) | (_| |  __/
//|_|  |_|\__,_|_|_| |_|  \____\___/ \__,_|\___|

int main() {
    
    char string[] = "6 2 3 + - 3 8 2 / + * 2 * 3 +";
    
    int length;
    length = strlen(string);
    length = (length+1)/2;
    char parsedString[length];
    
    readString(string, parsedString);
    
    printf("Read in String: ");
    printString(parsedString);
    

//------UNDERSTANDING STRINGS-------
// Scratch code.
    
//    char holder;
//    holder = *parsedString;
//    int digitChecker = atoi(&holder);
//    printf("%d ", digitChecker);
//    
//    if (isdigit(parsedString[2])) {
//        printf("itworks");
//    }
//    else {
//        printf("not a int");
//    }
//    
//    holder = *(parsedString+1);
//    digitChecker = atoi(&holder);
//    printf("%d ", digitChecker);
//    
//    printf("\n");
//    
//    printf("Length:%d\n", length);
//-----------------------------------
    
    solvePostFix(parsedString, length);

    TREE T = createTree(parsedString, length);
    
    
    printf("Infix Form: ");
    printInfix(T);
    printf("\n");
    
    printf("Prefix Form: ");
    printPrefix(T);
    printf("\n");
    
    return 0;
}


// _____                 _   _
//|  ___|   _ _ __   ___| |_(_) ___  _ __  ___
//| |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//|  _|| |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

//-----------------------
//  READING THE STRINGS
//-----------------------
void readString(char *str, char toStore[]) {
    int i=0;
    for (char *k=strtok(str, " "); k != NULL; k=strtok(NULL, " ")) {
        toStore[i] = *k;
        i++;
    }
}

//-----------------------
//  PRINTING THE STRINGS
//-----------------------
void printString(char *str) {
    int i = 0;
    while (str[i] != NULL) {
        printf("%c ", str[i]);
        i++;
    }
    printf("\n");
}

//-----------------------
//  SOLVE THE EXPRESSION
//-----------------------
int solvePostFix(char expression[], int length) {
    int stack[length];
    int digitChecker;
    int stackPtr = 0;
    int operand1;
    int operand2;
    char holder;
    
    for (int i=0; i<length; i++) {
        
        if (isdigit(expression[i])) {
            holder = *(expression+i);
            digitChecker = atoi(&holder);
            stack[stackPtr] = digitChecker;
            stackPtr++;
//            printf("%d ", stack[stackPtr-1]);
        }
        else {
//            printf("X ");
            operand2 = stack[stackPtr-1];
            stackPtr--;
            operand1 = stack[stackPtr-1];
            stackPtr--;
//            printf("\noperand1:%d\noperand2:%d\n", operand1, operand2);
            
            switch (expression[i]) {
                case '+':
                    stack[stackPtr] = operand1 + operand2;
                    break;
                case '-':
                    stack[stackPtr] = operand1 - operand2;
                    break;
                case '*':
                    stack[stackPtr] = operand1 * operand2;
                    break;
                case '/':
                    stack[stackPtr] = operand1 / operand2;
                    break;
                case '^':
                    stack[stackPtr] = operand1 ^ operand2;
                    break;
                case '%':
                    stack[stackPtr] = operand1 % operand2;
                    break;
                case '&':
                    stack[stackPtr] = operand1 & operand2;
                    break;
            }
            stackPtr++;
        }
    }
    
    printf("Answer: %d\n", stack[stackPtr-1]);
    return stack[stackPtr-1];
}

//--------------------------
//  CREATE EXPRESSION TREE
//--------------------------
TREE createTree(char expression[], int length) {
    
    int i = length-1;
    TREE stack[length];
    int stackPtr = 0;
    int prevIsNum = 0; // checks if the last value in expression is a number
    
    TREE T=(TREE)malloc(sizeof(struct NODE));
    T->element=expression[i];
    TREE currNode = T;
    i--;
    
    stack[stackPtr] = T;
    stackPtr++;
    
    while (i >= 0) {
        if (!prevIsNum) {
            if (isdigit(expression[i])) {
                TREE node = (TREE)malloc(sizeof(struct NODE));
                node->element = expression[i];
                i--;
                
                currNode->rightChild = node;
                node->rightChild = NULL;
                node->leftChild = NULL;
                
                currNode = stack[stackPtr-1];
                stackPtr--;
                prevIsNum = 1;
            }
            else {
                TREE node = (TREE)malloc(sizeof(struct NODE));
                node->element = expression[i];
                i--;
                
                currNode->rightChild = node;
                stack[stackPtr] = node;
                stackPtr++;
                
                currNode = node;
                prevIsNum=0;
            }
        }
        
        else {
            if (isdigit(expression[i])) {
                TREE node = (TREE)malloc(sizeof(struct NODE));
                node->element = expression[i];
                i--;
                
                currNode->leftChild = node;
                node->rightChild = NULL;
                node->leftChild = NULL;
                
                currNode = stack[stackPtr-1];
                stackPtr--;
                prevIsNum = 1;
            }
            else {
                TREE node = (TREE)malloc(sizeof(struct NODE));
                node->element = expression[i];
                i--;
                
                currNode->leftChild = node;
                stack[stackPtr] = node;
                stackPtr++;
                
                currNode = node;
                prevIsNum=0;
            }
        }
        
    }
    
    return T;
}

//-----------------------
//     PRINT INFIX
//-----------------------
void printInfix (TREE T) {
    if (T==NULL) {
        return;
    }
    printInfix(T->leftChild);
    printf("%c ", T->element);
    printInfix(T->rightChild);
    
}

//-----------------------
//    PRINT PREFIX
//-----------------------
void printPrefix (TREE T) {
    if (T==NULL) {
        return;
    }
    printf("%c ", T->element);
    printPrefix(T->leftChild);
    printPrefix(T->rightChild);
}

// Problems
// isdigit doesn't account for double digit numbers
// Also since using char array, I can only use integers from -9 to 9 in my expression
// Other than that, it works great!
// Tried using an int array instead of char array, but ran into a few problems
