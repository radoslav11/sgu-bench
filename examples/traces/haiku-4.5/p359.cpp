#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    cin.ignore(); // ignore newline after n
    
    // Map from pointer name to memory address it points to
    map<char, int> pointerToAddress;
    pointerToAddress['a'] = 0;
    pointerToAddress['b'] = 1;
    pointerToAddress['c'] = 2;
    pointerToAddress['d'] = 3;
    
    // Map from address to value stored there
    map<int, int> memory;
    memory[0] = 0;
    memory[1] = 0;
    memory[2] = 0;
    memory[3] = 0;
    
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        
        // Remove semicolon
        if (line.back() == ';') {
            line.pop_back();
        }
        
        if (line.find("writeln") != string::npos) {
            // writeln(pointer^)
            char ptr = line[8]; // the pointer char
            int addr = pointerToAddress[ptr];
            cout << memory[addr] << "\n";
        }
        else if (line.find(":=") != string::npos) {
            size_t assignPos = line.find(":=");
            string left = line.substr(0, assignPos);
            string right = line.substr(assignPos + 2);
            
            if (left.find('^') != string::npos) {
                // pointer^:=...
                char ptr = left[0];
                int addr = pointerToAddress[ptr];
                
                if (right.find('^') != string::npos) {
                    // pointer1^:=pointer2^
                    char ptr2 = right[0];
                    int addr2 = pointerToAddress[ptr2];
                    memory[addr] = memory[addr2];
                }
                else {
                    // pointer^:=constant
                    int value = stoi(right);
                    memory[addr] = value;
                }
            }
            else {
                // pointer1:=pointer2
                char ptr1 = left[0];
                char ptr2 = right[0];
                pointerToAddress[ptr1] = pointerToAddress[ptr2];
            }
        }
    }
    
    return 0;
}
