#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<string> grid(N);
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }

    vector<pair<int, int>> Q;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == '1') {
                Q.push_back({i, j});
            }
        }
    }

    if (Q.size() % 2 != 0) {
        cout << "NO" << endl;
        return 0;
    }

    // Define the four 90-degree rotations as functions: (r, c) -> (nr, nc)
    auto R0 = [](int r, int c) { return make_pair(r, c); };
    auto R90 = [](int r, int c) { return make_pair(-c, r); };
    auto R180 = [](int r, int c) { return make_pair(-r, -c); };
    auto R270 = [](int r, int c) { return make_pair(c, -r); };
    vector<decltype(R0)> rotations = {R0, R90, R180, R270};

    // Try each rotation
    for (auto& rot : rotations) {
        for (int i = 0; i < Q.size(); i++) {
            int pr = Q[i].first, pc = Q[i].second;
            auto rp = rot(pr, pc);
            int rpr = rp.first, rpc = rp.second;
            for (int j = 0; j < Q.size(); j++) {
                int qr = Q[j].first, qc = Q[j].second;
                int vr = qr - rpr;
                int vc = qc - rpc;

                vector<pair<int, int>> A;
                vector<vector<bool>> inA(N, vector<bool>(N, false));

                // Build set A: points x in Q such that rot(x) + (vr, vc) is in Q
                for (auto& x : Q) {
                    int xr = x.first, xc = x.second;
                    auto ry = rot(xr, xc);
                    int yr = ry.first + vr;
                    int yc = ry.second + vc;
                    
                    if (yr >= 0 && yr < N && yc >= 0 && yc < N && grid[yr][yc] == '1') {
                        A.push_back(x);
                        inA[xr][xc] = true;
                    }
                }

                // Check if size is exactly half of Q
                if (A.size() * 2 != Q.size()) {
                    continue;
                }

                // Check if A and rot(A) + v are disjoint
                bool has_intersection = false;
                for (auto& x : A) {
                    int xr = x.first, xc = x.second;
                    auto ry = rot(xr, xc);
                    int yr = ry.first + vr;
                    int yc = ry.second + vc;
                    if (inA[yr][yc]) {
                        has_intersection = true;
                        break;
                    }
                }

                if (!has_intersection) {
                    cout << "YES" << endl;
                    for (int r = 0; r < N; r++) {
                        string s(N, '0');
                        for (int c = 0; c < N; c++) {
                            if (inA[r][c]) {
                                s[c] = '1';
                            }
                        }
                        cout << s << endl;
                    }
                    return 0;
                }
            }
        }
    }

    cout << "NO" << endl;
    return 0;
}
