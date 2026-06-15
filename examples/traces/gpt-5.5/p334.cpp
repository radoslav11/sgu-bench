#include <bits/stdc++.h>
using namespace std;

struct Candidate {
    int omask, tmask;
};

int H, W;
vector<string> grid;
vector<pair<int,int>> cells;
vector<Candidate> cand;
vector<int> byCell[9];
vector<int> answer, cur;
const int FULL = (1 << 9) - 1;

bool connectedMask(int mask) {
    if (mask == 0) return false;

    int start = __builtin_ctz(mask);
    int seen = 0;
    queue<int> q;
    q.push(start);
    seen |= (1 << start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u = 0; u < 9; u++) {
            if (!(mask & (1 << u))) continue;
            if (seen & (1 << u)) continue;

            int dr = abs(cells[v].first - cells[u].first);
            int dc = abs(cells[v].second - cells[u].second);
            if (dr + dc == 1) {
                seen |= (1 << u);
                q.push(u);
            }
        }
    }

    return seen == mask;
}

bool dfs(int omask, int tmask, int left) {
    if (omask == FULL) {
        answer = cur;
        return true;
    }
    if (left == 0) return false;

    int remaining = 9 - __builtin_popcount((unsigned)omask);
    if (remaining < left) return false;

    int bestCell = -1;
    int bestCnt = INT_MAX;

    for (int i = 0; i < 9; i++) {
        if (omask & (1 << i)) continue;

        int cnt = 0;
        for (int id : byCell[i]) {
            const auto &c = cand[id];
            if ((c.omask & omask) == 0 && (c.tmask & tmask) == 0) cnt++;
        }

        if (cnt < bestCnt) {
            bestCnt = cnt;
            bestCell = i;
        }
    }

    if (bestCnt == 0) return false;

    for (int id : byCell[bestCell]) {
        const auto &c = cand[id];

        if (c.omask & omask) continue;
        if (c.tmask & tmask) continue;

        cur.push_back(id);
        if (dfs(omask | c.omask, tmask | c.tmask, left - 1)) return true;
        cur.pop_back();
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> H >> W;
    grid.resize(H);

    for (int r = 0; r < H; r++) {
        cin >> grid[r];
        for (int c = 0; c < W; c++) {
            if (grid[r][c] == 'X') {
                cells.push_back({r, c});
            }
        }
    }

    bool used[512][512];
    memset(used, 0, sizeof(used));

    for (int mask = 1; mask <= FULL; mask++) {
        if (!connectedMask(mask)) continue;

        for (int dr = -H - 3; dr <= H + 3; dr++) {
            for (int dc = -W - 3; dc <= W + 3; dc++) {
                int tmask = 0;
                bool ok = true;

                for (int i = 0; i < 9; i++) {
                    if (!(mask & (1 << i))) continue;

                    int nr = cells[i].first + dr;
                    int nc = cells[i].second + dc;

                    if (nr < 0 || nr >= 3 || nc < 0 || nc >= 3) {
                        ok = false;
                        break;
                    }

                    tmask |= (1 << (nr * 3 + nc));
                }

                if (ok && !used[mask][tmask]) {
                    used[mask][tmask] = true;
                    cand.push_back({mask, tmask});
                }
            }
        }
    }

    sort(cand.begin(), cand.end(), [](const Candidate &a, const Candidate &b) {
        return __builtin_popcount((unsigned)a.omask) > __builtin_popcount((unsigned)b.omask);
    });

    for (int id = 0; id < (int)cand.size(); id++) {
        for (int i = 0; i < 9; i++) {
            if (cand[id].omask & (1 << i)) {
                byCell[i].push_back(id);
            }
        }
    }

    int K = 9;

    for (int k = 1; k <= 9; k++) {
        cur.clear();
        answer.clear();

        if (dfs(0, 0, k)) {
            K = k;
            break;
        }
    }

    vector<string> original = grid;
    vector<string> target(3, string(3, '?'));

    for (int part = 0; part < K; part++) {
        char ch = char('A' + part);
        const auto &c = cand[answer[part]];

        for (int i = 0; i < 9; i++) {
            if (c.omask & (1 << i)) {
                original[cells[i].first][cells[i].second] = ch;
            }
        }

        for (int i = 0; i < 9; i++) {
            if (c.tmask & (1 << i)) {
                target[i / 3][i % 3] = ch;
            }
        }
    }

    cout << K << '\n';

    for (int r = 0; r < H; r++) {
        cout << original[r] << '\n';
    }

    cout << '\n';

    for (int r = 0; r < 3; r++) {
        cout << target[r] << '\n';
    }

    return 0;
}
