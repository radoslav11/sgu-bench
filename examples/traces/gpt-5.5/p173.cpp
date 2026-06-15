#include <bits/stdc++.h>
using namespace std;

struct Matrix {
    int n;
    vector<unsigned long long> row;

    Matrix(int n = 0, bool ident = false) : n(n), row(n, 0) {
        if (ident) {
            for (int i = 0; i < n; ++i) row[i] = 1ULL << i;
        }
    }
};

Matrix multiply(const Matrix& a, const Matrix& b) {
    int n = a.n;
    Matrix c(n);

    for (int i = 0; i < n; ++i) {
        unsigned long long res = 0;
        unsigned long long mask = a.row[i];

        while (mask) {
            int j = __builtin_ctzll(mask);
            res ^= b.row[j];
            mask &= mask - 1;
        }

        c.row[i] = res;
    }

    return c;
}

Matrix power(Matrix base, long long exp) {
    int n = base.n;
    Matrix res(n, true);

    while (exp > 0) {
        if (exp & 1) res = multiply(res, base);
        base = multiply(base, base);
        exp >>= 1;
    }

    return res;
}

unsigned long long applyMatrix(const Matrix& mat, unsigned long long vec) {
    unsigned long long res = 0;

    for (int i = 0; i < mat.n; ++i) {
        if (__builtin_parityll(mat.row[i] & vec)) {
            res |= 1ULL << i;
        }
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K, L;
    cin >> N >> M >> K >> L;

    vector<int> S(M);
    vector<long long> D(M);

    for (int i = 0; i < M; ++i) {
        cin >> S[i] >> D[i];
        --S[i];
    }

    int vars = K - 1;
    vector<unsigned long long> equations;

    for (int t = 0; t < L; ++t) {
        string before, after;
        cin >> before >> after;

        unsigned long long eq = 0;

        for (int i = 0; i < vars; ++i) {
            if (before[i + 1] == '1') {
                eq |= 1ULL << i;
            }
        }

        int rhs = (after[K - 1] - '0') ^ (before[0] - '0');
        if (rhs) eq |= 1ULL << vars;

        equations.push_back(eq);
    }

    vector<int> where(vars, -1);
    int row = 0;

    for (int col = 0; col < vars; ++col) {
        int sel = -1;

        for (int i = row; i < L; ++i) {
            if ((equations[i] >> col) & 1ULL) {
                sel = i;
                break;
            }
        }

        if (sel == -1) continue;

        swap(equations[row], equations[sel]);
        where[col] = row;

        for (int i = 0; i < L; ++i) {
            if (i != row && ((equations[i] >> col) & 1ULL)) {
                equations[i] ^= equations[row];
            }
        }

        ++row;
    }

    vector<int> A(vars, 0);
    for (int i = 0; i < vars; ++i) {
        A[i] = (equations[where[i]] >> vars) & 1ULL;
    }

    Matrix inverseX(K);

    inverseX.row[0] = 1ULL << (K - 1);
    for (int i = 0; i < vars; ++i) {
        if (A[i]) inverseX.row[0] |= 1ULL << i;
    }

    for (int i = 1; i < K; ++i) {
        inverseX.row[i] = 1ULL << (i - 1);
    }

    string coins;
    cin >> coins;

    vector<int> state(N);
    for (int i = 0; i < N; ++i) {
        state[i] = coins[i] - '0';
    }

    for (int op = M - 1; op >= 0; --op) {
        Matrix rollback = power(inverseX, D[op]);

        unsigned long long vec = 0;
        for (int i = 0; i < K; ++i) {
            if (state[S[op] + i]) {
                vec |= 1ULL << i;
            }
        }

        vec = applyMatrix(rollback, vec);

        for (int i = 0; i < K; ++i) {
            state[S[op] + i] = (vec >> i) & 1ULL;
        }
    }

    for (int i = 0; i < N; ++i) {
        cout << state[i];
    }
    cout << '\n';

    return 0;
}
