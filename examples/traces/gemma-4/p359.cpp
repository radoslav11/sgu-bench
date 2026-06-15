#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

/**
 * Problem: 359. Pointers
 * 
 * Pete has four pointers a, b, c, d that initially point to four distinct 
 * memory cells, each containing the value 0.
 * 
 * Operations:
 * 1. pointer^:=constant (e.g., a^:=5): sets the value at the memory cell 
 *    pointed to by the pointer to the given digit constant.
 * 2. pointer1^:=pointer2^ (e.g., c^:=a^): copies the value from the memory 
 *    cell pointed to by pointer2 to the memory cell pointed to by pointer1.
 * 3. pointer1:=pointer2 (e.g., b:=c): makes pointer1 point to the same 
 *    memory cell as pointer2.
 * 4. writeln(pointer^) (e.g., writeln(b^)): prints the value in the 
 *    memory cell pointed to by the pointer.
 * 
 * Input contains N lines of instructions.
 * N <= 30.
 */

int main() {
    // Speed up I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    string line;
    getline(cin, line); // consume the newline character remaining after reading N

    // ptr[char] will store the index (0-3) of the memory cell the pointer points to.
    int ptr[256];
    for (int i = 0; i < 256; ++i) ptr[i] = -1;
    ptr[(unsigned char)'a'] = 0;
    ptr[(unsigned char)'b'] = 1;
    ptr[(unsigned char)'c'] = 2;
    ptr[(unsigned char)'d'] = 3;

    // memory[index] stores the integer value stored in cell 'index'.
    int memory[4] = {0, 0, 0, 0};

    for (int i = 0; i < N; ) {
        if (!getline(cin, line)) break;

        // Remove any whitespaces from the command line to simplify parsing.
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());

        if (line.empty()) continue;

        // Determine the operator type and execute it.
        if (line.find("writeln") != string::npos) {
            // Type 4: writeln(pointer^);
            size_t pos = line.find('(');
            if (pos != string::npos && pos + 1 < line.length()) {
                char p = line[pos + 1];
                // Print the value in the memory cell currently pointed to by p.
                cout << memory[ptr[(unsigned char)p]] << "\n";
            }
        } else {
            size_t pos = line.find(":=");
            if (pos != string::npos) {
                if (pos >= 1 && line[pos - 1] == '^') {
                    // Type 1 (pointer^:=constant;) or Type 2 (pointer1^:=pointer2^;)
                    char p1 = line[pos - 2];
                    // Check if the value after ":=" is a digit constant (Type 1) or another pointer (Type 2).
                    if (pos + 2 < line.length() && isdigit((unsigned char)line[pos + 2])) {
                        // Type 1: p1^:=constant;
                        memory[ptr[(unsigned char)p1]] = line[pos + 2] - '0';
                    } else if (pos + 2 < line.length()) {
                        // Type 2: p1^:=p2^;
                        char p2 = line[pos + 2];
                        memory[ptr[(unsigned char)p1]] = memory[ptr[(unsigned char)p2]];
                    }
                } else if (pos >= 1) {
                    // Type 3: pointer1:=pointer2;
                    char p1 = line[pos - 1];
                    char p2 = line[pos + 2];
                    // Pointer p1 now points to the same cell as pointer p2.
                    ptr[(unsigned char)p1] = ptr[(unsigned char)p2];
                }
            }
        }
        i++; // Only increment i if a valid command was processed.
    }

    return 0;
}

