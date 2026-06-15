#include <iostream>
#include <string>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    char c;
    if (!(cin >> c)) return 0;
    
    string s;
    cin >> s;
    
    if (s.empty()) {
        cout << "NO\n";
        return 0;
    }
    
    char other = (c == 'A') ? 'B' : 'A';
    
    // According to the rules (and the known intended logic resolving a 
    // historical typo in the original problem statement), the first company 
    // can either end the sequence by placing the competitor's product, or 
    // place its own product and leave an empty stand for itself to fill later.
    // This results in a valid string being zero or more of the first company's 
    // stands, followed by exactly one stand of the competitor.
    if (s.back() != other) {
        cout << "NO\n";
        return 0;
    }
    
    for (size_t i = 0; i < s.length() - 1; ++i) {
        if (s[i] != c) {
            cout << "NO\n";
            return 0;
        }
    }
    
    cout << "YES\n";
    return 0;
}
