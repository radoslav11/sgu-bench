#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    int prevKey;
    short prevK;
    short curMask;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    cin >> N >> M >> K;

    vector<vector<int>> a(N, vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) cin >> a[i][j];
    }

    vector<pair<int,int>> masks; // {mask, length}
    masks.push_back({0, 0});
    for (int l = 0; l < M; l++) {
        int mask = 0;
        for (int r = l; r < M; r++) {
            mask |= (1 << r);
            masks.push_back({mask, r - l + 1});
        }
    }

    vector<vector<int>> rowSum(N, vector<int>(1 << M, 0));
    for (int i = 0; i < N; i++) {
        for (auto [mask, len] : masks) {
            int s = 0;
            for (int j = 0; j < M; j++) {
                if (mask & (1 << j)) s += a[i][j];
            }
            rowSum[i][mask] = s;
        }
    }

    const int NEG = -1e9;

    vector<vector<unordered_map<int, Node>>> layer(
        N + 1, vector<unordered_map<int, Node>>(K + 1)
    );

    layer[0][0][0] = {0, -1, -1, 0};

    for (int row = 0; row < N; row++) {
        for (int used = 0; used <= K; used++) {
            for (auto &it : layer[row][used]) {
                int key = it.first;
                Node curNode = it.second;

                int started = key >> M;
                int prevMask = key & ((1 << M) - 1);
                int closed = started & ~prevMask;

                for (auto [curMask, len] : masks) {
                    if (used + len > K) continue;

                    if (curMask & closed) continue;

                    if (prevMask && curMask && ((prevMask & curMask) == 0)) continue;

                    if (started && !prevMask && curMask) continue;

                    int newStarted = started | curMask;
                    int newKey = (newStarted << M) | curMask;
                    int newUsed = used + len;
                    int newVal = curNode.val + rowSum[row][curMask];

                    auto &mp = layer[row + 1][newUsed];
                    auto f = mp.find(newKey);
                    if (f == mp.end() || f->second.val < newVal) {
                        mp[newKey] = {newVal, key, (short)used, (short)curMask};
                    }
                }
            }
        }
    }

    int bestVal = NEG;
    int bestKey = 0;

    for (auto &it : layer[N][K]) {
        if (it.second.val > bestVal) {
            bestVal = it.second.val;
            bestKey = it.first;
        }
    }

    cout << "Oil : " << bestVal << '\n';

    vector<int> chosenMasks(N, 0);
    int row = N, used = K, key = bestKey;

    while (row > 0) {
        Node nd = layer[row][used][key];
        chosenMasks[row - 1] = nd.curMask;
        key = nd.prevKey;
        used = nd.prevK;
        row--;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (chosenMasks[i] & (1 << j)) {
                cout << i + 1 << ' ' << j + 1 << '\n';
            }
        }
    }

    return 0;
}
