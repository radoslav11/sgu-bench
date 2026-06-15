#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cstring>

using namespace std;

int n;
int8_t memo[105][105][105][7];

bool can_solve(int u, int l, int h, int D) {
    if (u + l + h <= 1) return D >= 0;
    if (D < 0) return false;
    
    long long possibilities = 2LL * u + l + h;
    long long max_states = 1;
    for (int i = 0; i < D; ++i) max_states *= 3;
    if (possibilities > max_states) return false;

    if (memo[u][l][h][D] != -1) return memo[u][l][h][D];
    
    int R_count = n - (u + l + h);
    for (int k = 1; k <= (u + l + h + R_count) / 2; ++k) {
        for (int u1 = 0; u1 <= min(u, k); ++u1) {
            for (int l1 = 0; l1 <= min(l, k - u1); ++l1) {
                int rem1 = k - u1 - l1;
                for (int h1 = 0; h1 <= min(h, rem1); ++h1) {
                    int sum1 = u1 + l1 + h1;
                    for (int u2 = 0; u2 <= min(u - u1, k); ++u2) {
                        for (int l2 = 0; l2 <= min(l - l1, k - u2); ++l2) {
                            int rem2 = k - u2 - l2;
                            for (int h2 = 0; h2 <= min(h - h1, rem2); ++h2) {
                                int sum2 = u2 + l2 + h2;
                                int r1 = k - sum1;
                                int r2 = k - sum2;
                                if (r1 < 0 || r2 < 0) continue;
                                if (r1 + r2 > R_count) continue;
                                
                                int P_lt = (l1 + u1) + (h2 + u2);
                                int P_eq = 2 * (u - u1 - u2) + (l - l1 - l2) + (h - h1 - h2);
                                int P_gt = (l2 + u2) + (h1 + u1);
                                
                                bool ok = true;
                                if (P_lt > 0 && !can_solve(0, l1 + u1, h2 + u2, D - 1)) ok = false;
                                if (ok && P_eq > 0 && !can_solve(u - u1 - u2, l - l1 - l2, h - h1 - h2, D - 1)) ok = false;
                                if (ok && P_gt > 0 && !can_solve(0, l2 + u2, h1 + u1, D - 1)) ok = false;
                                
                                if (ok) {
                                    return memo[u][l][h][D] = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return memo[u][l][h][D] = 0;
}

string join(const vector<int>& v) {
    string res;
    for (size_t i = 0; i < v.size(); ++i) {
        res += to_string(v[i]);
        if (i + 1 < v.size()) res += "+";
    }
    return res;
}

void print_tree(vector<int> U, vector<int> L, vector<int> H, int D, int indent) {
    if (U.size() + L.size() + H.size() <= 1) {
        int fake = -1;
        if (!U.empty()) fake = U[0];
        else if (!L.empty()) fake = L[0];
        else if (!H.empty()) fake = H[0];
        cout << string(indent, ' ') << "fake " << fake << "\n";
        return;
    }
    
    int R_count = n - (U.size() + L.size() + H.size());
    for (int k = 1; k <= (U.size() + L.size() + H.size() + R_count) / 2; ++k) {
        for (int u1 = 0; u1 <= min((int)U.size(), k); ++u1) {
            for (int l1 = 0; l1 <= min((int)L.size(), k - u1); ++l1) {
                int rem1 = k - u1 - l1;
                for (int h1 = 0; h1 <= min((int)H.size(), rem1); ++h1) {
                    int sum1 = u1 + l1 + h1;
                    for (int u2 = 0; u2 <= min((int)U.size() - u1, k); ++u2) {
                        for (int l2 = 0; l2 <= min((int)L.size() - l1, k - u2); ++l2) {
                            int rem2 = k - u2 - l2;
                            for (int h2 = 0; h2 <= min((int)H.size() - h1, rem2); ++h2) {
                                int sum2 = u2 + l2 + h2;
                                int r1 = k - sum1;
                                int r2 = k - sum2;
                                if (r1 < 0 || r2 < 0) continue;
                                if (r1 + r2 > R_count) continue;
                                
                                int P_lt = (l1 + u1) + (h2 + u2);
                                int P_eq = 2 * (U.size() - u1 - u2) + (L.size() - l1 - l2) + (H.size() - h1 - h2);
                                int P_gt = (l2 + u2) + (h1 + u1);
                                
                                bool ok = true;
                                if (P_lt > 0 && !can_solve(0, l1 + u1, h2 + u2, D - 1)) ok = false;
                                if (ok && P_eq > 0 && !can_solve(U.size() - u1 - u2, L.size() - l1 - l2, H.size() - h1 - h2, D - 1)) ok = false;
                                if (ok && P_gt > 0 && !can_solve(0, l2 + u2, h1 + u1, D - 1)) ok = false;
                                
                                if (ok) {
                                    vector<int> U1(U.begin(), U.begin() + u1);
                                    vector<int> U2(U.begin() + u1, U.begin() + u1 + u2);
                                    vector<int> Urem(U.begin() + u1 + u2, U.end());
                                    
                                    vector<int> L1(L.begin(), L.begin() + l1);
                                    vector<int> L2(L.begin() + l1, L.begin() + l1 + l2);
                                    vector<int> Lrem(L.begin() + l1 + l2, L.end());
                                    
                                    vector<int> H1(H.begin(), H.begin() + h1);
                                    vector<int> H2(H.begin() + h1, H.begin() + h1 + h2);
                                    vector<int> Hrem(H.begin() + h1 + h2, H.end());
                                    
                                    vector<int> Left, Right;
                                    vector<int> Real;
                                    vector<bool> is_candidate(n + 1, false);
                                    for(int x : U) is_candidate[x] = true;
                                    for(int x : L) is_candidate[x] = true;
                                    for(int x : H) is_candidate[x] = true;
                                    for(int i = 1; i <= n; ++i) {
                                        if (!is_candidate[i]) Real.push_back(i);
                                    }
                                    
                                    for(int i = 0; i < r1; ++i) Left.push_back(Real[i]);
                                    for(int i = r1; i < r1 + r2; ++i) Right.push_back(Real[i]);
                                    
                                    for(int x : U1) Left.push_back(x);
                                    for(int x : L1) Left.push_back(x);
                                    for(int x : H1) Left.push_back(x);
                                    
                                    for(int x : U2) Right.push_back(x);
                                    for(int x : L2) Right.push_back(x);
                                    for(int x : H2) Right.push_back(x);
                                    
                                    sort(Left.begin(), Left.end());
                                    sort(Right.begin(), Right.end());
                                    
                                    cout << string(indent, ' ') << "weigh " << join(Left) << " vs " << join(Right) << "\n";
                                    
                                    if (P_lt > 0) {
                                        cout << string(indent, ' ') << "case <:\n";
                                        vector<int> new_U = Urem;
                                        vector<int> new_L = L1; new_L.insert(new_L.end(), U1.begin(), U1.end());
                                        vector<int> new_H = H2; new_H.insert(new_H.end(), U2.begin(), U2.end());
                                        sort(new_U.begin(), new_U.end());
                                        sort(new_L.begin(), new_L.end());
                                        sort(new_H.begin(), new_H.end());
                                        print_tree(new_U, new_L, new_H, D - 1, indent + 2);
                                        cout << string(indent, ' ') << "end\n";
                                    }
                                    
                                    if (P_eq > 0) {
                                        cout << string(indent, ' ') << "case =:\n";
                                        vector<int> new_U = Urem;
                                        vector<int> new_L = Lrem;
                                        vector<int> new_H = Hrem;
                                        sort(new_U.begin(), new_U.end());
                                        sort(new_L.begin(), new_L.end());
                                        sort(new_H.begin(), new_H.end());
                                        print_tree(new_U, new_L, new_H, D - 1, indent + 2);
                                        cout << string(indent, ' ') << "end\n";
                                    }
                                    
                                    if (P_gt > 0) {
                                        cout << string(indent, ' ') << "case >:\n";
                                        vector<int> new_U = Urem;
                                        vector<int> new_L = L2; new_L.insert(new_L.end(), U2.begin(), U2.end());
                                        vector<int> new_H = H1; new_H.insert(new_H.end(), U1.begin(), U1.end());
                                        sort(new_U.begin(), new_U.end());
                                        sort(new_L.begin(), new_L.end());
                                        sort(new_H.begin(), new_H.end());
                                        print_tree(new_U, new_L, new_H, D - 1, indent + 2);
                                        cout << string(indent, ' ') << "end\n";
                                    }
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if (!(cin >> n)) return 0;
    
    memset(memo, -1, sizeof(memo));
    int min_D = 1;
    while (!can_solve(n, 0, 0, min_D)) {
        min_D++;
    }
    
    cout << "need " << min_D << " weighings\n";
    vector<int> U(n);
    iota(U.begin(), U.end(), 1);
    print_tree(U, {}, {}, min_D, 0);
    
    return 0;
}
