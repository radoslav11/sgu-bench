#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n, a, b;
    cin >> n >> a >> b;
    string s;
    cin >> s;

    // Check if it's impossible: need at least a+b characters, but we only have n
    if (a + b > n) {
        cout << "-1" << endl;
        return 0;
    }

    int t = n - a - b; // number of '2's required

    // Count current occurrences of '0', '1', '2'
    int c0 = 0, c1 = 0;
    for (char c : s) {
        if (c == '0') c0++;
        else if (c == '1') c1++;
    }
    int c2 = n - c0 - c1;

    // Calculate minimum replacements needed
    int min_rep = n - (min(c0, a) + min(c1, b) + min(c2, t));

    string ans = s;
    vector<int> to_change;

    int kept0 = 0, kept1 = 0, kept2 = 0;

    // First pass: keep as many correct characters as possible
    for (int i = 0; i < n; i++) {
        if (s[i] == '0' && kept0 < a) {
            kept0++;
        } else if (s[i] == '1' && kept1 < b) {
            kept1++;
        } else if (s[i] == '2' && kept2 < t) {
            kept2++;
        } else {
            to_change.push_back(i);
        }
    }

    int need0 = a - kept0;
    int need1 = b - kept1;
    int need2 = t - kept2;

    // Second pass: assign missing characters to the positions that were not kept
    for (int idx : to_change) {
        if (need0 > 0) {
            ans[idx] = '0';
            need0--;
        } else if (need1 > 0) {
            ans[idx] = '1';
            need1--;
        } else if (need2 > 0) {
            ans[idx] = '2';
            need2--;
        }
    }

    cout << min_rep << endl;
    cout << ans << endl;

    return 0;
}
