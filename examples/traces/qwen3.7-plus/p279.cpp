#pragma GCC optimize("O3")
#include <iostream>
#include <vector>

using namespace std;

int n;
vector<int> a;
vector<int> P;
vector<bool> used;
vector<int> placed;

bool solve(int step) {
    if (step == 0) return true;
    for (int x = 1; x <= n; ++x) {
        if (used[x]) continue;
        int R = 0, count_less = 0;
        for (int p : placed) {
            if (p > x) R++;
            else count_less++;
        }
        int L = a[x] - R;
        if (L >= 0 && L <= count_less) {
            used[x] = true;
            placed.push_back(x);
            P[step] = x;
            if (solve(step - 1)) return true;
            placed.pop_back();
            used[x] = false;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> n)) return 0;
    
    a.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    
    P.resize(n + 1);
    used.assign(n + 1, false);
    
    if (solve(n)) {
        cout << "YES\n";
        
        vector<vector<bool>> after(n + 1, vector<bool>(n + 1, false));
        for (int k = 1; k <= n; ++k) {
            int x = P[k];
            vector<int> candidates;
            int R = 0;
            for (int m = k + 1; m <= n; ++m) {
                if (P[m] > x) {
                    R++;
                } else {
                    candidates.push_back(m);
                }
            }
            int L = a[x] - R;
            int start_idx = candidates.size() - L;
            for (int i = start_idx; i < (int)candidates.size(); ++i) {
                after[k][candidates[i]] = true;
            }
        }

        vector<int> ans;
        ans.reserve(2 * n);
        int p_ptr = 1;
        for (int k = 1; k <= n; ++k) {
            while (p_ptr <= n) {
                if (after[k][p_ptr]) {
                    break;
                }
                ans.push_back(-P[p_ptr]);
                p_ptr++;
            }
            ans.push_back(P[k]);
        }
        while (p_ptr <= n) {
            ans.push_back(-P[p_ptr]);
            p_ptr++;
        }

        for (int i = 0; i < (int)ans.size(); ++i) {
            cout << ans[i] << (i + 1 == (int)ans.size() ? "" : " ");
        }
        cout << "\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
