#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

int N;
vector<string> grid;
vector<pair<int,int>> Q_points;

bool check(int k, int vx, int vy, vector<pair<int,int>>& out_A) {
    vector<vector<int>> color(N, vector<int>(N, 0));
    queue<pair<int,int>> q;
    
    auto in_Q = [&](int r, int c) {
        return r >= 0 && r < N && c >= 0 && c < N && grid[r][c] == '1';
    };
    
    auto get_R = [&](int r, int c) {
        if (k == 0) return make_pair(r, c);
        if (k == 1) return make_pair(-c, r);
        if (k == 2) return make_pair(-r, -c);
        if (k == 3) return make_pair(c, -r);
        return make_pair(r, c);
    };
    
    auto get_inv_R = [&](int r, int c) {
        if (k == 0) return make_pair(r, c);
        if (k == 1) return make_pair(c, -r);
        if (k == 2) return make_pair(-r, -c);
        if (k == 3) return make_pair(-c, r);
        return make_pair(r, c);
    };
    
    auto get_T = [&](int r, int c) {
        auto p = get_R(r, c);
        return make_pair(p.first + vx, p.second + vy);
    };
    
    auto get_inv_T = [&](int r, int c) {
        auto p = get_inv_R(r - vx, c - vy);
        return make_pair(p.first, p.second);
    };
    
    for (auto& p : Q_points) {
        int r = p.first, c = p.second;
        auto nxt = get_T(r, c);
        auto prv = get_inv_T(r, c);
        
        bool nxt_in = in_Q(nxt.first, nxt.second);
        bool prv_in = in_Q(prv.first, prv.second);
        
        if (!nxt_in && !prv_in) {
            return false;
        }
        if (!nxt_in) {
            if (color[r][c] == 1) return false;
            color[r][c] = 2;
            q.push({r, c});
        }
        if (!prv_in) {
            if (color[r][c] == 2) return false;
            color[r][c] = 1;
            q.push({r, c});
        }
    }
    
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        int r = curr.first, c = curr.second;
        int c_col = color[r][c];
        
        auto nxt = get_T(r, c);
        if (in_Q(nxt.first, nxt.second)) {
            if (color[nxt.first][nxt.second] == c_col) return false;
            if (color[nxt.first][nxt.second] == 0) {
                color[nxt.first][nxt.second] = 3 - c_col;
                q.push({nxt.first, nxt.second});
            }
        }
        
        auto prv = get_inv_T(r, c);
        if (in_Q(prv.first, prv.second)) {
            if (color[prv.first][prv.second] == c_col) return false;
            if (color[prv.first][prv.second] == 0) {
                color[prv.first][prv.second] = 3 - c_col;
                q.push({prv.first, prv.second});
            }
        }
    }
    
    for (auto& p : Q_points) {
        if (color[p.first][p.second] == 0) {
            int r = p.first, c = p.second;
            color[r][c] = 1;
            q.push({r, c});
            while (!q.empty()) {
                auto curr = q.front();
                q.pop();
                int cr = curr.first, cc = curr.second;
                int c_col = color[cr][cc];
                
                auto nxt = get_T(cr, cc);
                if (in_Q(nxt.first, nxt.second)) {
                    if (color[nxt.first][nxt.second] == c_col) return false;
                    if (color[nxt.first][nxt.second] == 0) {
                        color[nxt.first][nxt.second] = 3 - c_col;
                        q.push({nxt.first, nxt.second});
                    }
                }
                
                auto prv = get_inv_T(cr, cc);
                if (in_Q(prv.first, prv.second)) {
                    if (color[prv.first][prv.second] == c_col) return false;
                    if (color[prv.first][prv.second] == 0) {
                        color[prv.first][prv.second] = 3 - c_col;
                        q.push({prv.first, prv.second});
                    }
                }
            }
        }
    }
    
    out_A.clear();
    for (auto& p : Q_points) {
        if (color[p.first][p.second] == 1) {
            out_A.push_back(p);
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> N)) return 0;
    
    grid.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] == '1') {
                Q_points.push_back({i, j});
            }
        }
    }
    
    if (Q_points.empty() || Q_points.size() % 2 != 0) {
        cout << "NO\n";
        return 0;
    }
    
    for (int k = 0; k < 4; ++k) {
        vector<pair<int,int>> V;
        auto a = Q_points[0];
        
        auto get_R = [&](int r, int c) {
            if (k == 0) return make_pair(r, c);
            if (k == 1) return make_pair(-c, r);
            if (k == 2) return make_pair(-r, -c);
            if (k == 3) return make_pair(c, -r);
            return make_pair(r, c);
        };
        
        for (auto& b : Q_points) {
            auto Ra = get_R(a.first, a.second);
            auto Rb = get_R(b.first, b.second);
            V.push_back({b.first - Ra.first, b.second - Ra.second});
            V.push_back({a.first - Rb.first, a.second - Rb.second});
        }
        
        sort(V.begin(), V.end());
        V.erase(unique(V.begin(), V.end()), V.end());
        
        for (auto& v : V) {
            vector<pair<int,int>> out_A;
            if (check(k, v.first, v.second, out_A)) {
                cout << "YES\n";
                vector<string> ans(N, string(N, '0'));
                for (auto& p : out_A) {
                    ans[p.first][p.second] = '1';
                }
                for (int i = 0; i < N; ++i) {
                    cout << ans[i] << "\n";
                }
                return 0;
            }
        }
    }
    
    cout << "NO\n";
    return 0;
}
