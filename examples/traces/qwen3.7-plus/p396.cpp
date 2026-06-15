#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

const int INF = 1e9;
int dp[1005][4][4][4];
short parent_state[1005][4][4][4];

int char_to_int(char c) {
    if (c == 'L') return 0;
    if (c == 'U') return 1;
    if (c == 'R') return 2;
    if (c == 'D') return 3;
    if (c == 'N') return 4;
    return 4;
}

char int_to_char(int i) {
    if (i == 0) return 'L';
    if (i == 1) return 'U';
    if (i == 2) return 'R';
    if (i == 3) return 'D';
    return 'N';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;
    string S;
    cin >> S;

    for (int i = 0; i <= N; ++i) {
        for (int L = 0; L < 4; ++L) {
            for (int R = 0; R < 4; ++R) {
                for (int mask = 0; mask < 4; ++mask) {
                    dp[i][L][R][mask] = INF;
                    parent_state[i][L][R][mask] = -1;
                }
            }
        }
    }

    dp[0][0][2][0] = 0;

    for (int i = 1; i <= N; ++i) {
        int req = char_to_int(S[i-1]);

        for (int pL = 0; pL < 4; ++pL) {
            for (int pR = 0; pR < 4; ++pR) {
                if (pL == pR) continue;
                if (pL == 2 && pR == 0) continue;

                for (int pmask = 0; pmask < 4; ++pmask) {
                    if (dp[i-1][pL][pR][pmask] == INF) continue;

                    int current_cost = dp[i-1][pL][pR][pmask];

                    auto update = [&](int nL, int nR, int nmask, int add_cost) {
                        if (current_cost + add_cost < dp[i][nL][nR][nmask]) {
                            dp[i][nL][nR][nmask] = current_cost + add_cost;
                            parent_state[i][nL][nR][nmask] = (short)((pL << 4) | (pR << 2) | pmask);
                        }
                    };

                    if (req == 4) {
                        update(pL, pR, 0, 0);
                    } else {
                        if (req == pL) {
                            update(pL, pR, 1, 1);
                        }
                        if (req == pR) {
                            update(pL, pR, 2, 1);
                        }

                        int nL1 = req, nR1 = pR;
                        if (nL1 != pL && nL1 != nR1 && !(nL1 == 2 && nR1 == 0)) {
                            int cost = (pmask & 1) ? 9 : 3;
                            update(nL1, nR1, 1, cost);
                        }

                        int nL2 = pL, nR2 = req;
                        if (nR2 != pR && nL2 != nR2 && !(nL2 == 2 && nR2 == 0)) {
                            int cost = (pmask & 2) ? 9 : 3;
                            update(nL2, nR2, 2, cost);
                        }

                        for (int nL = 0; nL < 4; ++nL) {
                            for (int nR = 0; nR < 4; ++nR) {
                                if (nL == nR) continue;
                                if (nL == 2 && nR == 0) continue;
                                if (nL != req && nR != req) continue;
                                if (nL == pL || nR == pR) continue;
                                update(nL, nR, 3, 10);
                            }
                        }
                    }
                }
            }
        }
    }

    int min_cost = INF;
    int best_L = -1, best_R = -1, best_mask = -1;

    for (int L = 0; L < 4; ++L) {
        for (int R = 0; R < 4; ++R) {
            if (L == R) continue;
            if (L == 2 && R == 0) continue;
            for (int mask = 0; mask < 4; ++mask) {
                if (dp[N][L][R][mask] < min_cost) {
                    min_cost = dp[N][L][R][mask];
                    best_L = L;
                    best_R = R;
                    best_mask = mask;
                }
            }
        }
    }

    cout << min_cost << "\n";

    vector<array<int, 2>> path(N);
    int cL = best_L, cR = best_R, cmask = best_mask;
    for (int i = N; i >= 1; --i) {
        path[i-1] = {cL, cR};
        int p = parent_state[i][cL][cR][cmask];
        cmask = p & 3;
        cR = (p >> 2) & 3;
        cL = (p >> 4) & 3;
    }

    for (int i = 0; i < N; ++i) {
        cout << int_to_char(path[i][0]) << int_to_char(path[i][1]) << "\n";
    }

    return 0;
}
