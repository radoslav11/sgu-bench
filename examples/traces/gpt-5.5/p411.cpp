#include <bits/stdc++.h>
using namespace std;

static unsigned char pal[2000][2000];
static unsigned short bestLen[2001][2001];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    cin >> a >> b;

    int n = (int)a.size();
    int m = (int)b.size();

    for (int len = 1; len <= n; ++len) {
        for (int l = 0; l + len <= n; ++l) {
            int r = l + len - 1;
            if (a[l] == a[r] && (len <= 2 || pal[l + 1][r - 1])) {
                pal[l][r] = 1;
                bestLen[r + 1][len] = len;
            }
        }
    }

    for (int end = 1; end <= n; ++end) {
        for (int len = 1; len <= n; ++len) {
            if (bestLen[end][len] < bestLen[end][len - 1]) {
                bestLen[end][len] = bestLen[end][len - 1];
            }
        }
    }

    vector<unsigned short> prev(m + 1, 0), cur(m + 1, 0);

    int ansLen = 0;
    int ansStart = 0;

    for (int i = 1; i <= n; ++i) {
        cur[0] = 0;
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                cur[j] = prev[j - 1] + 1;
                int commonLen = cur[j];

                if (commonLen > ansLen) {
                    int candidateLen = bestLen[i][commonLen];
                    if (candidateLen > ansLen) {
                        ansLen = candidateLen;
                        ansStart = i - candidateLen;
                    }
                }
            } else {
                cur[j] = 0;
            }
        }
        swap(prev, cur);
    }

    cout << a.substr(ansStart, ansLen) << '\n';

    return 0;
}
