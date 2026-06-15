#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Parent {
    int prev;
    Parent(int p = -1) : prev(p) {}
};

int id(int l, int r, int last) {
    return ((l * 4 + r) * 4 + last);
}

void decode(int s, int &l, int &r, int &last) {
    last = s % 4;
    s /= 4;
    r = s % 4;
    l = s / 4;
}

bool legalPos(int l, int r) {
    if (l == r) return false;
    if (l == 2 && r == 0) return false; // left leg on RIGHT, right leg on LEFT
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string song;
    cin >> N >> song;

    auto btn = [](char c) {
        if (c == 'L') return 0;
        if (c == 'U') return 1;
        if (c == 'R') return 2;
        if (c == 'D') return 3;
        return -1;
    };

    const char outChar[4] = {'L', 'U', 'R', 'D'};

    vector<vector<Parent>> par(N + 1, vector<Parent>(64));
    vector<int> dp(64, INF), ndp(64, INF);

    int start = id(0, 2, 0); // left leg LEFT, right leg RIGHT, no previous press
    dp[start] = 0;

    for (int beat = 1; beat <= N; beat++) {
        fill(ndp.begin(), ndp.end(), INF);
        int need = btn(song[beat - 1]);

        auto relax = [&](int ns, int ps, int cost) {
            if (dp[ps] + cost < ndp[ns]) {
                ndp[ns] = dp[ps] + cost;
                par[beat][ns] = Parent(ps);
            }
        };

        for (int s = 0; s < 64; s++) {
            if (dp[s] == INF) continue;

            int l, r, last;
            decode(s, l, r, last);

            // 1. Do nothing, press nothing
            if (need == -1) {
                relax(id(l, r, 0), s, 0);
            }

            // 2. Do not move, press one of the occupied buttons
            if (need == -1 || need == l) {
                relax(id(l, r, 1), s, 1);
            }
            if (need == -1 || need == r) {
                relax(id(l, r, 2), s, 1);
            }

            // 3. Move the leg which didn't press at previous beat
            vector<int> canMoveCheap;
            if (last == 0) {
                canMoveCheap.push_back(1); // left leg
                canMoveCheap.push_back(2); // right leg
            } else if (last == 1) {
                canMoveCheap.push_back(2);
            } else if (last == 2) {
                canMoveCheap.push_back(1);
            }

            for (int leg : canMoveCheap) {
                if (leg == 1) {
                    for (int nl = 0; nl < 4; nl++) {
                        if (nl == r) continue;
                        if (!legalPos(nl, r)) continue;
                        if (need == -1 || need == nl) {
                            relax(id(nl, r, 1), s, 3);
                        }
                    }
                } else {
                    for (int nr = 0; nr < 4; nr++) {
                        if (nr == l) continue;
                        if (!legalPos(l, nr)) continue;
                        if (need == -1 || need == nr) {
                            relax(id(l, nr, 2), s, 3);
                        }
                    }
                }
            }

            // 4. Move the leg which pressed at previous beat
            vector<int> canMoveExpensive;
            if (last == 1) {
                canMoveExpensive.push_back(1);
            } else if (last == 2) {
                canMoveExpensive.push_back(2);
            } else if (last == 3) {
                canMoveExpensive.push_back(1);
                canMoveExpensive.push_back(2);
            }

            for (int leg : canMoveExpensive) {
                if (leg == 1) {
                    for (int nl = 0; nl < 4; nl++) {
                        if (nl == r) continue;
                        if (!legalPos(nl, r)) continue;
                        if (need == -1 || need == nl) {
                            relax(id(nl, r, 1), s, 9);
                        }
                    }
                } else {
                    for (int nr = 0; nr < 4; nr++) {
                        if (nr == l) continue;
                        if (!legalPos(l, nr)) continue;
                        if (need == -1 || need == nr) {
                            relax(id(l, nr, 2), s, 9);
                        }
                    }
                }
            }

            // 5. Jump with both legs
            for (int nl = 0; nl < 4; nl++) {
                for (int nr = 0; nr < 4; nr++) {
                    if (!legalPos(nl, nr)) continue;
                    if (need == -1 || need == nl || need == nr) {
                        relax(id(nl, nr, 3), s, 10);
                    }
                }
            }
        }

        dp.swap(ndp);
    }

    int bestState = -1;
    int bestCost = INF;

    for (int s = 0; s < 64; s++) {
        if (dp[s] < bestCost) {
            bestCost = dp[s];
            bestState = s;
        }
    }

    vector<int> states(N + 1);
    states[N] = bestState;

    for (int i = N; i >= 1; i--) {
        states[i - 1] = par[i][states[i]].prev;
    }

    cout << bestCost << '\n';

    for (int i = 1; i <= N; i++) {
        int l, r, last;
        decode(states[i], l, r, last);
        cout << outChar[l] << outChar[r] << '\n';
    }

    return 0;
}
