#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;

int H, W;
int B[305][305];
int S[305][305];
int C0[305];
int L_arr[305], R_arr[305];
int A[305][305];

bool dfs(int k, bitset<305> valid0, bitset<305> valid1) {
    if (k == H - 1) return true;
    for (int c = L_arr[k]; c <= R_arr[k]; ++c) {
        bitset<305> next0, next1;
        bool ok = true;
        for (int j = 0; j < W; ++j) {
            int cj = (j % 2 == 0) ? c + S[k][j] : -c + S[k][j];
            if (cj < 0 || cj > 2) {
                ok = false;
                break;
            }
            bool n0 = false, n1 = false;
            if (valid0[j]) {
                if (cj == 0) n0 = true;
                if (cj == 1) n1 = true;
            }
            if (valid1[j]) {
                if (cj == 1) n0 = true;
                if (cj == 2) n1 = true;
            }
            if (!n0 && !n1) {
                ok = false;
                break;
            }
            if (n0) next0[j] = 1;
            if (n1) next1[j] = 1;
        }
        if (ok) {
            C0[k] = c;
            if (dfs(k + 1, next0, next1)) return true;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if (!(cin >> H >> W)) return 0;
    
    for (int i = 0; i < H - 1; ++i) {
        string s;
        cin >> s;
        for (int j = 0; j < W - 1; ++j) {
            B[i][j] = s[j] - '0';
        }
    }
    
    for (int k = 0; k < H - 1; ++k) {
        S[k][0] = 0;
        for (int j = 1; j < W; ++j) {
            S[k][j] = B[k][j - 1] - S[k][j - 1];
        }
        
        int L = 0, R = 2;
        for (int j = 0; j < W; ++j) {
            if (j % 2 == 0) {
                L = max(L, -S[k][j]);
                R = min(R, 2 - S[k][j]);
            } else {
                L = max(L, S[k][j] - 2);
                R = min(R, S[k][j]);
            }
        }
        if (L > R) {
            cout << "CORRUPT\n";
            return 0;
        }
        L_arr[k] = L;
        R_arr[k] = R;
    }
    
    bitset<305> init0, init1;
    for (int j = 0; j < W; ++j) {
        init0[j] = 1;
        init1[j] = 1;
    }
    
    if (!dfs(0, init0, init1)) {
        cout << "CORRUPT\n";
        return 0;
    }
    
    for (int j = 0; j < W; ++j) {
        bool found = false;
        for (int start = 0; start <= 1; ++start) {
            bool ok = true;
            A[0][j] = start;
            for (int k = 0; k < H - 1; ++k) {
                int cj = (j % 2 == 0) ? C0[k] + S[k][j] : -C0[k] + S[k][j];
                int nxt = cj - A[k][j];
                if (nxt < 0 || nxt > 1) {
                    ok = false;
                    break;
                }
                A[k + 1][j] = nxt;
            }
            if (ok) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "CORRUPT\n";
            return 0;
        }
    }
    
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            cout << A[i][j];
        }
        cout << "\n";
    }
    
    return 0;
}
