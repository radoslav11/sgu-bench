#include <bits/stdc++.h>
using namespace std;

int L;
vector<int> nb, a;

bool ok_partial() {
    for (int sh = 1; sh < L; ++sh) {
        if (a[sh] == -1) continue;
        if (a[sh] == 0) continue; // rotations starting with 0 are not considered

        for (int j = 0; j < L; ++j) {
            int x = a[j];
            int y = a[(sh + j) % L];
            if (x == -1 || y == -1) break;

            if (x > y) break;
            if (x < y) return false;
        }
    }
    return true;
}

bool ok_full() {
    for (int sh = 1; sh < L; ++sh) {
        if (a[sh] == 0) continue;

        for (int j = 0; j < L; ++j) {
            int x = a[j];
            int y = a[(sh + j) % L];

            if (x > y) break;
            if (x < y) return false;
        }
    }
    return true;
}

bool dfs(int pos, bool tight) {
    if (pos == L) return ok_full();

    int lim = tight ? nb[pos] : 1;

    for (int bit = lim; bit >= 0; --bit) {
        if (pos == 0 && bit == 0) continue;

        a[pos] = bit;

        if (ok_partial()) {
            if (dfs(pos + 1, tight && (bit == lim))) return true;
        }

        a[pos] = -1;
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long n;
    cin >> n;

    string s;
    {
        unsigned long long x = n;
        while (x) {
            s.push_back(char('0' + (x & 1)));
            x >>= 1;
        }
        reverse(s.begin(), s.end());
    }

    L = (int)s.size();
    nb.resize(L);
    for (int i = 0; i < L; ++i) nb[i] = s[i] - '0';

    a.assign(L, -1);

    dfs(0, true);

    unsigned long long ans = 0;
    for (int i = 0; i < L; ++i) {
        ans = ans * 2 + a[i];
    }

    cout << ans << '\n';

    return 0;
}
