#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string S;
    if (!(cin >> S)) return 0;
    
    int n = S.length();
    vector<int> pref(n, 0);
    int current_pref = 0;
    for (int i = 0; i < n; ++i) {
        if (S[i] == '(') {
            current_pref++;
        } else {
            current_pref--;
        }
        pref[i] = current_pref;
    }
    
    int best_i = -1;
    for (int i = n - 1; i >= 0; --i) {
        if (S[i] == '(' && pref[i] >= 2) {
            best_i = i;
            break;
        }
    }
    
    if (best_i == -1) {
        cout << "No solution\n";
    } else {
        int open_count = 0;
        int close_count = 0;
        for (int j = 0; j < best_i; ++j) {
            if (S[j] == '(') {
                open_count++;
            } else {
                close_count++;
            }
        }
        
        int rem_open = n / 2 - open_count;
        int rem_close = n / 2 - close_count - 1;
        
        string ans = S.substr(0, best_i);
        ans += ')';
        ans.append(rem_open, '(');
        ans.append(rem_close, ')');
        
        cout << ans << "\n";
    }
    
    return 0;
}
