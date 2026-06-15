#include <iostream>
#include <vector>
#include <string>

using namespace std;

void solve() {
    int H, W, N;
    cin >> H >> W >> N;
    
    vector<pair<int, int>> parity0, parity1;
    for (int r = 1; r <= H - 2; ++r) {
        for (int c = 1; c <= W - 2; ++c) {
            if ((r + c) % 2 == 0) {
                parity0.push_back({r, c});
            } else {
                parity1.push_back({r, c});
            }
        }
    }
    
    vector<pair<int, int>>* target = nullptr;
    if (parity0.size() >= (size_t)N) {
        target = &parity0;
    } else if (parity1.size() >= (size_t)N) {
        target = &parity1;
    } else {
        cout << "Impossible\n";
        return;
    }
    
    vector<string> grid(H, string(W, '.'));
    for (int i = 0; i < N; ++i) {
        grid[target->at(i).first][target->at(i).second] = '#';
    }
    
    for (int r = 0; r < H; ++r) {
        cout << grid[r] << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    if (cin >> T) {
        for (int t = 0; t < T; ++t) {
            if (t > 0) cout << "\n";
            solve();
        }
    }
    return 0;
}
