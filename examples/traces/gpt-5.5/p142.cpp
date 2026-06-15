#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string s;
    cin >> N >> s;

    const int MAXL = 20;
    vector<int> seen(1 << MAXL, 0);

    for (int len = 1; ; len++) {
        int mask = (1 << len) - 1;
        int val = 0;

        for (int i = 0; i < N; i++) {
            int bit = (s[i] == 'b');
            val = ((val << 1) | bit) & mask;

            if (i >= len - 1) {
                seen[val] = len;
            }
        }

        for (int x = 0; x < (1 << len); x++) {
            if (seen[x] != len) {
                string ans;
                for (int i = len - 1; i >= 0; i--) {
                    ans += ((x >> i) & 1) ? 'b' : 'a';
                }

                cout << len << '\n' << ans << '\n';
                return 0;
            }
        }
    }

    return 0;
}
