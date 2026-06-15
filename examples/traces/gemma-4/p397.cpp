#include <cstdio>

/**
 * Problem Analysis:
 * The task is to simulate a simple text editor that supports inserting characters, 
 * moving the cursor left, and moving the cursor right. 
 * 
 * Data Structures:
 * A simple std::string::insert approach would take O(N^2) time, which is too slow 
 * for a string of length 10^6. 
 * 
 * To solve this efficiently, we can use a two-stack approach. 
 * One stack (left_stack) will contain characters to the left of the cursor, 
 * and another stack (right_stack) will contain characters to the right of the cursor.
 * 
 * Operations:
 * - Insert character 'a'-'z': Push the character onto the left_stack.
 * - 'L' (move left): Pop the top of the left_stack and push it onto the right_stack.
 * - 'R' (move right): Pop the top of the right_stack and push it onto the left_stack.
 * 
 * Time Complexity: 
 * Each operation is O(1), so the total time complexity is O(N), 
 * where N is the length of the input string.
 * 
 * Space Complexity:
 * We use three arrays of size 10^6, which takes about 3MB. 
 * This is well within the 64MB limit.
 */

const int MAXN = 1000005;
char left_stack[MAXN];
char right_stack[MAXN];
char input_str[MAXN];

int main() {
    // Read input string using scanf for faster I/O
    if (scanf("%s", input_str) != 1) {
        return 0;
    }

    int L_top = 0; // Pointer to the top of the left stack
    int R_top = 0; // Pointer to the top of the right stack

    for (int i = 0; input_str[i] != '\0'; ++i) {
        char c = input_str[i];
        if (c == 'L') {
            // Move cursor left: pop from left_stack, push to right_stack
            if (L_top > 0) {
                right_stack[R_top++] = left_stack[--L_top];
            }
        } else if (c == 'R') {
            // Move cursor right: pop from right_stack, push to left_stack
            if (R_top > 0) {
                left_stack[L_top++] = right_stack[--R_top];
            }
        } else {
            // Insert character: push to left_stack
            left_stack[L_top++] = c;
        }
    }

    // Output the word
    // First, print all characters in the left stack (from bottom to top)
    for (int i = 0; i < L_top; ++i) {
        putchar(left_stack[i]);
    }
    // Then, print all characters in the right stack (from top to bottom)
    // Since characters were pushed into right_stack such that the rightmost 
    // character of the word is at index 0 of right_stack, we iterate backwards.
    for (int i = R_top - 1; i >= 0; --i) {
        putchar(right_stack[i]);
    }
    putchar('\n');

    return 0;
}

