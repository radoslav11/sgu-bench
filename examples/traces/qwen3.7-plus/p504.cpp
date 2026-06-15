#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int row_rad[705][1405];
int col_rad[1405][705];

int st_row[11][1405][705];
int st_col[11][1405][705];
int log2_val[1405];

void build_rmq(int h, int w) {
    log2_val[1] = 0;
    for (int i = 2; i <= 1400; i++) {
        log2_val[i] = log2_val[i / 2] + 1;
    }
    
    for (int k = 1; k <= 2 * w + 1; k++) {
        for (int i = 1; i <= h; i++) {
            st_row[0][k][i] = row_rad[i][k];
        }
        for (int p = 1; (1 << p) <= h; p++) {
            for (int i = 1; i + (1 << p) - 1 <= h; i++) {
                st_row[p][k][i] = min(st_row[p-1][k][i], st_row[p-1][k][i + (1 << (p-1))]);
            }
        }
    }
    
    for (int k = 1; k <= 2 * h + 1; k++) {
        for (int j = 1; j <= w; j++) {
            st_col[0][k][j] = col_rad[k][j];
        }
        for (int p = 1; (1 << p) <= w; p++) {
            for (int j = 1; j + (1 << p) - 1 <= w; j++) {
                st_col[p][k][j] = min(st_col[p-1][k][j], st_col[p-1][k][j + (1 << (p-1))]);
            }
        }
    }
}

int query_row(int k, int l, int r) {
    int p = log2_val[r - l + 1];
    return min(st_row[p][k][l], st_row[p][k][r - (1 << p) + 1]);
}

int query_col(int k, int l, int r) {
    int p = log2_val[r - l + 1];
    return min(st_col[p][k][l], st_col[p][k][r - (1 << p) + 1]);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int h, w;
    if (!(cin >> h >> w)) return 0;
    
    vector<string> M(h + 1);
    for (int i = 1; i <= h; i++) {
        cin >> M[i];
        M[i] = " " + M[i];
    }
    
    for (int i = 1; i <= h; i++) {
        string S = "#";
        for (int j = 1; j <= w; j++) {
            S += M[i][j];
            S += "#";
        }
        vector<int> d(S.length(), 0);
        int l = 0, r = -1;
        for (int k = 0; k < (int)S.length(); k++) {
            int k1 = (r >= k) ? min(d[l + r - k], r - k + 1) : 1;
            while (k - k1 >= 0 && k + k1 < (int)S.length() && S[k - k1] == S[k + k1]) {
                k1++;
            }
            d[k] = k1;
            if (k + k1 - 1 > r) {
                l = k - k1 + 1;
                r = k + k1 - 1;
            }
            row_rad[i][k + 1] = d[k] - 1;
        }
    }
    
    for (int j = 1; j <= w; j++) {
        string S = "#";
        for (int i = 1; i <= h; i++) {
            S += M[i][j];
            S += "#";
        }
        vector<int> d(S.length(), 0);
        int l = 0, r = -1;
        for (int k = 0; k < (int)S.length(); k++) {
            int k1 = (r >= k) ? min(d[l + r - k], r - k + 1) : 1;
            while (k - k1 >= 0 && k + k1 < (int)S.length() && S[k - k1] == S[k + k1]) {
                k1++;
            }
            d[k] = k1;
            if (k + k1 - 1 > r) {
                l = k - k1 + 1;
                r = k + k1 - 1;
            }
            col_rad[k + 1][j] = d[k] - 1;
        }
    }
    
    build_rmq(h, w);
    
    int global_best_n = 0;
    int best_r = 1, best_c = 1;
    
    for (int kr = 1; kr <= 2 * h + 1; kr++) {
        for (int kc = 1; kc <= 2 * w + 1; kc++) {
            if (kr % 2 != kc % 2) continue;
            
            int max_n = min({kr, kc, 2 * h - kr + 1, 2 * w - kc + 1});
            if (max_n % 2 != (kr + 1) % 2) {
                max_n--;
            }
            if (max_n < 1) continue;
            
            int low = 0, high = (max_n - 1) / 2;
            int best_n = 0;
            
            while (low <= high) {
                int mid = low + (high - low) / 2;
                int n = max_n - 2 * mid;
                
                int r = (kr - n + 1) / 2;
                int c = (kc - n + 1) / 2;
                
                bool ok = true;
                if (query_row(kc, r, r + n - 1) < n) ok = false;
                if (ok && query_col(kr, c, c + n - 1) < n) ok = false;
                
                if (ok) {
                    best_n = n;
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }
            
            if (best_n > global_best_n) {
                global_best_n = best_n;
                best_r = (kr - best_n + 1) / 2;
                best_c = (kc - best_n + 1) / 2;
            }
        }
    }
    
    cout << best_r << " " << best_c << " " << best_r + global_best_n - 1 << " " << best_c + global_best_n - 1 << "\n";
    
    return 0;
}
