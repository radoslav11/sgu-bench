#include <iostream>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
using namespace std;

int main() {
    string s1, s2;
    cin >> s1 >> s2;
    
    int n = s1.length();
    
    // Count the number of '+' in both strings
    int count1 = 0, count2 = 0;
    for (char c : s1) {
        if (c == '+') count1++;
    }
    for (char c : s2) {
        if (c == '+') count2++;
    }
    
    if (count1 != count2) {
        cout << -1 << endl;
        return 0;
    }
    
    vector<int> pos1, pos2;
    // Record positions of '+' in s1
    for (int i = 0; i < n; i++) {
        if (s1[i] == '+') {
            pos1.push_back(i);
        }
    }
    // Record positions of '+' in s2
    for (int i = 0; i < n; i++) {
        if (s2[i] == '+') {
            pos2.push_back(i);
        }
    }
    
    long long ans = 0;
    for (int i = 0; i < count1; i++) {
        ans += abs(pos1[i] - pos2[i]);
    }
    
    cout << ans << endl;
    
    return 0;
}
