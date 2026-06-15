#include <bits/stdc++.h>
using namespace std;

struct Choice {
    int dmask;
    int unmatched;
    int cost;
};

int N;
int A, Vallow;
vector<Choice> choices;

void gen(int c, int occ, int dmask, int cost) {
    while (c < N && (((A & (1 << c)) == 0) || (occ & (1 << c)))) c++;
    if (c >= N) {
        int unmatched = A & ~occ;
        if ((unmatched & (unmatched << 1)) == 0) {
            choices.push_back({dmask, unmatched, cost});
        }
        return;
    }

    int bit = 1 << c;

    // Leave this cell unmatched
    gen(c + 1, occ, dmask, cost);

    // Place vertical domino downwards
    if (Vallow & bit) {
        gen(c + 1, occ | bit, dmask | bit, cost + 1);
    }

    // Place horizontal domino
    if (c + 1 < N) {
        int bit2 = 1 << (c + 1);
        if ((A & bit2) && !(occ & bit2)) {
            gen(c + 2, occ | bit | bit2, dmask, cost + 1);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M;
    cin >> M >> N;

    vector<int> emptyMask(M + 1, 0);
    for (int i = 0; i < M; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < N; j++) {
            if (s[j] == '.') emptyMask[i] |= (1 << j);
        }
    }

    int S = 1 << N;
    int SZ = 1 << (2 * N);
    const int INF = 1e9;

    vector<int> dp(SZ, INF), ndp(SZ, INF);
    dp[0] = 0;

    for (int r = 0; r < M; r++) {
        fill(ndp.begin(), ndp.end(), INF);

        int curEmpty = emptyMask[r];
        int nextEmpty = emptyMask[r + 1];

        for (int up = 0; up < S; up++) {
            if ((up & ~curEmpty) != 0) continue;

            A = curEmpty & ~up;
            Vallow = A & nextEmpty;

            choices.clear();
            gen(0, 0, 0, 0);

            for (int prevUnmatched = 0; prevUnmatched < S; prevUnmatched++) {
                int oldIdx = (up << N) | prevUnmatched;
                if (dp[oldIdx] == INF) continue;

                int base = dp[oldIdx];

                for (const Choice &ch : choices) {
                    if (prevUnmatched & ch.unmatched) continue;

                    int newIdx = (ch.dmask << N) | ch.unmatched;
                    ndp[newIdx] = min(ndp[newIdx], base + ch.cost);
                }
            }
        }

        dp.swap(ndp);
    }

    int ans = INF;
    for (int lastUnmatched = 0; lastUnmatched < S; lastUnmatched++) {
        ans = min(ans, dp[lastUnmatched]);
    }

    cout << ans << '\n';

    return 0;
}
