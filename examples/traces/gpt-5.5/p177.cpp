#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    int W = (N + 63) / 64;
    vector<vector<unsigned long long>> grid(N, vector<unsigned long long>(W, ~0ULL));

    if (N % 64 != 0) {
        unsigned long long lastMask = (1ULL << (N % 64)) - 1;
        for (int i = 0; i < N; ++i) grid[i][W - 1] = lastMask;
    }

    for (int op = 0; op < M; ++op) {
        int x1, y1, x2, y2;
        char c;
        cin >> x1 >> y1 >> x2 >> y2 >> c;

        int r1 = min(x1, x2) - 1;
        int r2 = max(x1, x2) - 1;
        int col1 = min(y1, y2) - 1;
        int col2 = max(y1, y2) - 1;

        int w1 = col1 / 64;
        int w2 = col2 / 64;
        int b1 = col1 % 64;
        int b2 = col2 % 64;

        for (int r = r1; r <= r2; ++r) {
            if (w1 == w2) {
                unsigned long long mask;
                if (b2 == 63) mask = (~0ULL << b1);
                else mask = ((1ULL << (b2 + 1)) - 1) & (~0ULL << b1);

                if (c == 'w') grid[r][w1] |= mask;
                else grid[r][w1] &= ~mask;
            } else {
                unsigned long long firstMask = ~0ULL << b1;
                unsigned long long lastMask = (b2 == 63 ? ~0ULL : ((1ULL << (b2 + 1)) - 1));

                if (c == 'w') {
                    grid[r][w1] |= firstMask;
                    for (int w = w1 + 1; w < w2; ++w) grid[r][w] = ~0ULL;
                    grid[r][w2] |= lastMask;
                } else {
                    grid[r][w1] &= ~firstMask;
                    for (int w = w1 + 1; w < w2; ++w) grid[r][w] = 0ULL;
                    grid[r][w2] &= ~lastMask;
                }
            }
        }
    }

    long long white = 0;
    for (int i = 0; i < N; ++i) {
        for (int w = 0; w < W; ++w) {
            white += __builtin_popcountll(grid[i][w]);
        }
    }

    cout << white << '\n';

    return 0;
}
