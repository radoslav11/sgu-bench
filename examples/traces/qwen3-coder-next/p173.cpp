#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> mat_mult(vector<vector<int>> a, vector<vector<int>> b, int k) {
    vector<vector<int>> c(k, vector<int>(k, 0));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            int res = 0;
            for (int l = 0; l < k; l++) {
                if (a[i][l] && b[l][j]) {
                    res ^= 1;
                }
            }
            c[i][j] = res;
        }
    }
    return c;
}

vector<vector<int>> mat_pow(vector<vector<int>> base, long long exp, int k) {
    vector<vector<int>> res(k, vector<int>(k, 0));
    for (int i = 0; i < k; i++) {
        res[i][i] = 1;
    }
    while (exp) {
        if (exp & 1) {
            res = mat_mult(res, base, k);
        }
        base = mat_mult(base, base, k);
        exp >>= 1;
    }
    return res;
}

vector<vector<int>> get_inverse(vector<vector<int>> T, int k) {
    int n = k;
    vector<vector<int>> aug(n, vector<int>(2 * n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug[i][j] = T[i][j];
        }
        aug[i][n + i] = 1;
    }

    for (int col = 0; col < n; col++) {
        int pivot = -1;
        for (int r = col; r < n; r++) {
            if (aug[r][col] == 1) {
                pivot = r;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(aug[col], aug[pivot]);
        for (int r = 0; r < n; r++) {
            if (r != col && aug[r][col] == 1) {
                for (int j = 0; j < 2 * n; j++) {
                    aug[r][j] ^= aug[col][j];
                }
            }
        }
    }

    vector<vector<int>> inv(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = aug[i][n + j];
        }
    }
    return inv;
}

int main() {
    int N, M, K, L;
    cin >> N >> M >> K >> L;

    vector<pair<int, long long>> ops;
    for (int i = 0; i < M; i++) {
        int s;
        long long d;
        cin >> s >> d;
        ops.push_back({s, d});
    }

    vector<string> before(L), after(L);
    for (int i = 0; i < L; i++) {
        cin >> before[i] >> after[i];
    }

    string final_state;
    cin >> final_state;

    int n_vars = K - 1;
    vector<vector<int>> aug(L, vector<int>(n_vars + 1, 0));

    for (int i = 0; i < L; i++) {
        string s1 = before[i];
        string s2 = after[i];
        int b_val = (s2[K - 1] - '0') ^ (s1[0] - '0');
        aug[i][n_vars] = b_val;

        for (int j = 0; j < n_vars; j++) {
            aug[i][j] = s1[j + 1] - '0';
        }
    }

    vector<int> A_vec(n_vars, 0);
    vector<int> where(n_vars, -1);
    int r = 0;
    for (int col = 0; col < n_vars; col++) {
        int pivot = -1;
        for (int i = r; i < L; i++) {
            if (aug[i][col] == 1) {
                pivot = i;
                break;
            }
        }
        if (pivot == -1) continue;
        where[col] = r;
        swap(aug[r], aug[pivot]);
        for (int i = 0; i < L; i++) {
            if (i != r && aug[i][col] == 1) {
                for (int j = col; j <= n_vars; j++) {
                    aug[i][j] ^= aug[r][j];
                }
            }
        }
        r++;
    }

    for (int col = 0; col < n_vars; col++) {
        if (where[col] != -1) {
            A_vec[col] = aug[where[col]][n_vars];
        }
    }

    vector<vector<int>> T_mat(K, vector<int>(K, 0));
    for (int i = 0; i < K - 1; i++) {
        T_mat[i][i + 1] = 1;
    }
    T_mat[K - 1][0] = 1;
    for (int j = 1; j < K; j++) {
        T_mat[K - 1][j] = A_vec[j - 1];
    }

    vector<vector<int>> invT = get_inverse(T_mat, K);

    vector<bool> state(N);
    for (int i = 0; i < N; i++) {
        state[i] = (final_state[i] == '1');
    }

    for (int i = M - 1; i >= 0; i--) {
        int start = ops[i].first - 1;
        long long D = ops[i].second;

        vector<int> seg(K);
        for (int j = 0; j < K; j++) {
            seg[j] = state[start + j] ? 1 : 0;
        }

        vector<vector<int>> B = mat_pow(invT, D, K);

        vector<int> new_seg(K, 0);
        for (int i_row = 0; i_row < K; i_row++) {
            for (int j = 0; j < K; j++) {
                if (B[i_row][j]) {
                    new_seg[i_row] ^= seg[j];
                }
            }
        }

        for (int j = 0; j < K; j++) {
            state[start + j] = new_seg[j];
        }
    }

    for (int i = 0; i < N; i++) {
        cout << (state[i] ? '1' : '0');
    }
    cout << endl;

    return 0;
}
