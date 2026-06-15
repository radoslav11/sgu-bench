#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    cin >> s;
    int n = s.size();
    
    // Compute prefix balance: bal[i] = balance before position i
    vector<int> bal(n+1, 0);
    for (int i = 0; i < n; i++) {
        bal[i+1] = bal[i] + (s[i] == '(' ? 1 : -1);
    }
    
    // Find rightmost '(' that can be changed to ')'
    int pos = -1;
    for (int i = n-1; i >= 0; i--) {
        if (s[i] == '(') {
            int b = bal[i]; // balance before position i
            if (b >= 1) {
                int new_bal = b - 1;
                int rem = n - 1 - i;
                if (rem >= new_bal && (rem - new_bal) % 2 == 0) {
                    pos = i;
                    break;
                }
            }
        }
    }
    
    if (pos == -1) {
        cout << "No solution" << endl;
    } else {
        s[pos] = ')';
        int new_bal = bal[pos] - 1;
        int rem = n - 1 - pos;
        int opens = (rem - new_bal) / 2;
        int closes = new_bal + opens;
        for (int i = pos+1; i < n; i++) {
            if (opens > 0) { s[i] = '('; opens--; }
            else { s[i] = ')'; }
        }
        cout << s << endl;
    }
    return 0;
}
