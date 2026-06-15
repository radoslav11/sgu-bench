#include <iostream>
#include <string>
#include <deque>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string commands;
    std::getline(std::cin, commands);
    
    // We'll use two deques/vectors: one for the text before cursor, one for the text after cursor.
    // This gives O(1) amortized time for insertions and cursor movements.
    std::string left;  // Characters before cursor (left part, in correct order)
    std::string right; // Characters after cursor (right part, in reverse order so that front is immediate next char)
    
    for (char c : commands) {
        if (c >= 'a' && c <= 'z') {
            // Insert character: add to end of left, and cursor moves right (so nothing changes in right)
            left.push_back(c);
        } else if (c == 'L') {
            // Move cursor left: take last char from left and move to front of right
            if (!left.empty()) {
                right.push_back(left.back());
                left.pop_back();
            }
        } else if (c == 'R') {
            // Move cursor right: take front char from right and move to end of left
            if (!right.empty()) {
                left.push_back(right.back());
                right.pop_back();
            }
        }
    }
    
    // The final string is left + reverse of right
    std::cout << left;
    for (auto it = right.rbegin(); it != right.rend(); ++it) {
        std::cout << *it;
    }
    std::cout << '\n';
    
    return 0;
}
