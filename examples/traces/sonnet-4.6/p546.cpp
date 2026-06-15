#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, a, b;
    cin >> n >> a >> b;
    string s;
    cin >> s;
    
    int c = n - a - b;
    if (c < 0) {
        cout << -1 << endl;
        return 0;
    }
    
    int rem_a = a, rem_b = b, rem_c = c;
    string result = s;
    vector<int> to_replace;
    
    for (int i = 0; i < n; i++) {
        if (s[i] == '0' && rem_a > 0) {
            rem_a--;
        } else if (s[i] == '1' && rem_b > 0) {
            rem_b--;
        } else if (s[i] == '2' && rem_c > 0) {
            rem_c--;
        } else {
            to_replace.push_back(i);
        }
    }
    
    int t = (int)to_replace.size();
    cout << t << "\n";
    
    int idx = 0;
    for (int i = 0; i < rem_a; i++) {
        result[to_replace[idx++]] = '0';
    }
    for (int i = 0; i < rem_b; i++) {
        result[to_replace[idx++]] = '1';
    }
    for (int i = 0; i < rem_c; i++) {
        result[to_replace[idx++]] = '2';
    }
    
    cout << result << "\n";
    return 0;
}
