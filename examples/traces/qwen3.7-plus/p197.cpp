#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Matrix {
    int mat[32][32];
    int size;
    Matrix(int n, int p) {
        size = n;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                mat[i][j] = 0;
    }
};

Matrix multiply(const Matrix &A, const Matrix &B, int p) {
    int n = A.size;
    Matrix C(n, p);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            long long sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += (long long)A.mat[i][k] * B.mat[k][j];
            }
            C.mat[i][j] = sum % p;
        }
    }
    return C;
}

bool ok(int mask1, int mask2, int M) {
    for (int j = 0; j < M - 1; ++j) {
        int a = (mask1 >> j) & 3;
        int b = (mask2 >> j) & 3;
        if ((a == 0 && b == 0) || (a == 3 && b == 3)) {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string N_str;
    int M, P;
    if (!(cin >> N_str >> M >> P)) return 0;

    int i = (int)N_str.length() - 1;
    while (i >= 0 && N_str[i] == '0') {
        N_str[i] = '9';
        i--;
    }
    if (i >= 0) {
        N_str[i]--;
    }

    int start = 0;
    while (start < (int)N_str.length() - 1 && N_str[start] == '0') {
        start++;
    }
    N_str = N_str.substr(start);

    int S = 1 << M;
    Matrix res(S, P);
    for (int i = 0; i < S; ++i) res.mat[i][i] = 1;

    Matrix base(S, P);
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            if (ok(i, j, M)) {
                base.mat[i][j] = 1;
            }
        }
    }

    vector<Matrix> base_pow(10, Matrix(S, P));
    for (int i = 0; i < S; ++i) base_pow[0].mat[i][i] = 1;
    for (int d = 1; d <= 9; ++d) {
        base_pow[d] = multiply(base_pow[d - 1], base, P);
    }

    for (char c : N_str) {
        int d = c - '0';
        
        Matrix temp = res;
        res = Matrix(S, P);
        for (int i = 0; i < S; ++i) res.mat[i][i] = 1;
        for (int k = 0; k < 10; ++k) {
            res = multiply(res, temp, P);
        }
        
        if (d > 0) {
            res = multiply(res, base_pow[d], P);
        }
    }

    long long ans = 0;
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            ans = (ans + res.mat[i][j]) % P;
        }
    }

    cout << ans << "\n";
    return 0;
}
