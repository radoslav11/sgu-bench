#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;
const ull INF = 1000000000000000000ULL;

int K;
ull N;
uint32_t FULLMASK;
uint32_t adjMask[31];
int degv[31];

void addClique(const vector<int>& v) {
    for (int i = 0; i < (int)v.size(); ++i) {
        for (int j = i + 1; j < (int)v.size(); ++j) {
            int a = v[i] - 1, b = v[j] - 1;
            adjMask[a] |= (1u << b);
            adjMask[b] |= (1u << a);
        }
    }
}

struct Solver {
    int val[31];
    uint32_t forbid[31];
    int leftCnt;
    ull cap;

    ull dfs() {
        if (leftCnt == 0) return 1;

        int best = -1;
        int bestCnt = 100;

        for (int i = 0; i < 31; ++i) {
            if (val[i] == 0) {
                uint32_t avail = FULLMASK & ~forbid[i];
                int pc = __builtin_popcount(avail);
                if (pc == 0) return 0;
                if (pc < bestCnt || (pc == bestCnt && degv[i] > degv[best])) {
                    bestCnt = pc;
                    best = i;
                }
            }
        }

        uint32_t avail = FULLMASK & ~forbid[best];
        ull res = 0;

        while (avail) {
            uint32_t bit = avail & -avail;
            avail -= bit;
            int color = __builtin_ctz(bit) + 1;

            val[best] = color;
            --leftCnt;

            vector<pair<int, uint32_t>> changed;
            uint32_t nb = adjMask[best];

            while (nb) {
                uint32_t b = nb & -nb;
                nb -= b;
                int u = __builtin_ctz(b);
                if (val[u] == 0 && !(forbid[u] & bit)) {
                    changed.push_back({u, forbid[u]});
                    forbid[u] |= bit;
                }
            }

            ull got = dfs();
            res += got;

            for (auto &p : changed) forbid[p.first] = p.second;
            ++leftCnt;
            val[best] = 0;

            if (res >= cap) return cap;
        }

        return res;
    }

    ull count(const vector<int>& fixed, ull needCap) {
        memset(val, 0, sizeof(val));
        memset(forbid, 0, sizeof(forbid));
        leftCnt = 31;
        cap = needCap;

        for (int i = 0; i < 31; ++i) {
            if (fixed[i] != 0) {
                int c = fixed[i];
                if (c < 1 || c > K) return 0;
                uint32_t bit = 1u << (c - 1);

                uint32_t nb = adjMask[i];
                while (nb) {
                    uint32_t b = nb & -nb;
                    nb -= b;
                    int u = __builtin_ctz(b);
                    if (fixed[u] == c) return 0;
                }

                val[i] = c;
                --leftCnt;
            }
        }

        for (int i = 0; i < 31; ++i) {
            if (val[i] != 0) {
                uint32_t bit = 1u << (val[i] - 1);
                uint32_t nb = adjMask[i];
                while (nb) {
                    uint32_t b = nb & -nb;
                    nb -= b;
                    int u = __builtin_ctz(b);
                    if (val[u] == 0) forbid[u] |= bit;
                }
            }
        }

        return dfs();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> K >> N;
    FULLMASK = (1u << K) - 1u;

    vector<int> given(31);
    for (int i = 0; i < 31; ++i) cin >> given[i];

    vector<vector<int>> groups = {
        {1, 2},
        {3, 4, 5, 6, 7},
        {8, 9, 10, 11, 12, 13},
        {14, 15, 16, 17, 18},
        {19, 20, 21, 22, 23, 24},
        {25, 26, 27, 28, 29},
        {30, 31},

        {3, 8},
        {1, 4, 9, 14, 19},
        {2, 5, 10, 15, 20, 25},
        {6, 11, 16, 21, 26},
        {7, 12, 17, 22, 27, 30},
        {13, 18, 23, 28, 31},
        {24, 29},

        {7, 13},
        {2, 6, 12, 18, 24},
        {1, 5, 11, 17, 23, 29},
        {4, 10, 16, 22, 28},
        {3, 9, 15, 21, 27, 31},
        {8, 14, 20, 26, 30},
        {19, 25},

        {1, 2, 4, 5, 6, 10, 11},
        {3, 4, 8, 9, 10, 14, 15},
        {6, 7, 11, 12, 13, 17, 18},
        {10, 11, 15, 16, 17, 21, 22},
        {14, 15, 19, 20, 21, 25, 26},
        {17, 18, 22, 23, 24, 28, 29},
        {21, 22, 26, 27, 28, 30, 31}
    };

    for (auto &g : groups) addClique(g);
    for (int i = 0; i < 31; ++i) degv[i] = __builtin_popcount(adjMask[i]);

    Solver solver;
    vector<int> answer(31, 0);

    for (int pos = 0; pos < 31; ++pos) {
        bool chosen = false;

        int from = 1, to = K;
        if (given[pos] != 0) from = to = given[pos];

        for (int c = from; c <= to; ++c) {
            if (given[pos] != 0 && c != given[pos]) continue;

            vector<int> fixed(31, 0);
            for (int i = 0; i < pos; ++i) fixed[i] = answer[i];
            fixed[pos] = c;
            for (int i = pos + 1; i < 31; ++i) {
                if (given[i] != 0) fixed[i] = given[i];
            }

            ull cnt = solver.count(fixed, N);

            if (cnt >= N) {
                answer[pos] = c;
                chosen = true;
                break;
            } else {
                N -= cnt;
            }
        }

        if (!chosen) {
            cout << "No way\n";
            return 0;
        }
    }

    cout << "Found\n";
    for (int i = 0; i < 31; ++i) {
        if (i) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';

    return 0;
}
