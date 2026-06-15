#include <bits/stdc++.h>
using namespace std;

int main() {
    string n;
    int k;
    cin >> n >> k;
    
    // Greedy: remove K digits to maximize the number
    // Use a stack approach
    string result;
    int remove = k;
    
    for (char c : n) {
        while (remove > 0 && !result.empty() && result.back() < c) {
            result.pop_back();
            remove--;
        }
        result.push_back(c);
    }
    
    // If we still need to remove digits, remove from the end
    while (remove > 0) {
        result.pop_back();
        remove--;
    }
    
    // Remove leading zeros
    int start = 0;
    while (start < (int)result.size() - 1 && result[start] == '0') {
        start++;
    }
    
    cout << result.substr(start) << endl;
    
    return 0;
}
