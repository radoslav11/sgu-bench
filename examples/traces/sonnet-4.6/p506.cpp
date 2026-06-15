#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string text, msg;
    cin >> text >> msg;
    
    int n = text.size();
    int m = msg.size();
    
    vector<array<int,26>> nxt(n+1);
    for (int c = 0; c < 26; c++) nxt[n][c] = n;
    for (int i = n-1; i >= 0; i--) {
        for (int c = 0; c < 26; c++) nxt[i][c] = nxt[i+1][c];
        nxt[i][text[i]-'a'] = i;
    }
    
    long long ans = 0;
    for (int l = 0; l < n; l++) {
        int pos = l;
        bool ok = true;
        for (int j = 0; j < m; j++) {
            int c = msg[j] - 'a';
            if (nxt[pos][c] == n) { ok = false; break; }
            pos = nxt[pos][c] + 1;
        }
        if (ok) {
            int min_r = pos - 1;
            ans += (long long)(n - min_r);
        }
    }
    
    cout << ans << endl;
    return 0;
}
