#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int L, N;
vector<int> limdig;
int maskNormal[10], maskLead[10];

struct Solver {
    unordered_map<unsigned long long, int64> memo;

    unsigned long long key(int pos, int diff, int m1, int m2) {
        unsigned long long k = pos;
        k = (k << 7) | (unsigned long long)(diff + 50);
        k = (k << 19) | (unsigned long long)m1;
        k = (k << 19) | (unsigned long long)m2;
        return k;
    }

    int64 dfs(int pos, int diff, int m1, int m2, bool tight) {
        if (pos == L) {
            if (diff == 0) return 0;

            bool ok = false;
            if (-9 <= -diff && -diff <= 9) {
                int bit = (-diff) + 9;
                if (m1 & (1 << bit)) ok = true;
            }
            if (-9 <= diff && diff <= 9) {
                int bit = diff + 9;
                if (m2 & (1 << bit)) ok = true;
            }
            return ok ? 1 : 0;
        }

        if (!tight) {
            unsigned long long k = key(pos, diff, m1, m2);
            auto it = memo.find(k);
            if (it != memo.end()) return it->second;
        }

        int upper = tight ? limdig[pos] : 9;
        int lower = (pos == 0 ? 1 : 0);

        int64 res = 0;

        for (int d = lower; d <= upper; ++d) {
            bool ntight = tight && (d == upper);

            int ndiff = diff;
            int nm1 = m1;
            int nm2 = m2;

            if (pos < N) {
                ndiff += d;
                nm1 |= (pos == 0 ? maskLead[d] : maskNormal[d]);
            } else {
                ndiff -= d;
                nm2 |= maskNormal[d];
            }

            res += dfs(pos + 1, ndiff, nm1, nm2, ntight);
        }

        if (!tight) {
            unsigned long long k = key(pos, diff, m1, m2);
            memo[k] = res;
        }

        return res;
    }
};

int64 countEvenLenLE(int len, const string& bound) {
    L = len;
    N = L / 2;
    limdig.clear();
    for (char c : bound) limdig.push_back(c - '0');

    Solver solver;
    return solver.dfs(0, 0, 0, 0, true);
}

int64 countUpTo(int64 x) {
    if (x < 10) return 0;

    string s = to_string(x);
    int len = (int)s.size();

    int64 ans = 0;

    for (int l = 2; l < len; l += 2) {
        string bound(l, '9');
        ans += countEvenLenLE(l, bound);
    }

    if (len % 2 == 0) {
        ans += countEvenLenLE(len, s);
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int d = 0; d <= 9; ++d) {
        maskNormal[d] = 0;
        maskLead[d] = 0;

        for (int y = 0; y <= 9; ++y) {
            if (y != d) {
                int delta = y - d;
                maskNormal[d] |= (1 << (delta + 9));
            }
        }

        for (int y = 1; y <= 9; ++y) {
            if (y != d) {
                int delta = y - d;
                maskLead[d] |= (1 << (delta + 9));
            }
        }
    }

    int64 A, B;
    cin >> A >> B;

    cout << countUpTo(B) - countUpTo(A - 1) << '\n';

    return 0;
}
