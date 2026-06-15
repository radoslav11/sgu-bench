#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

// Global modulus for use within the matrix multiplication function
int P_val;

/**
 * A small Matrix structure for up to 32x32 matrices.
 * Uses long long for intermediate calculations to prevent overflow.
 */
struct Matrix {
    int sz;
    long long mat[32][32];

    Matrix(int s) : sz(s) {
        memset(mat, 0, sizeof(mat));
    }

    // Returns the identity matrix of size s
    static Matrix identity(int s) {
        Matrix res(s);
        for (int i = 0; i < s; ++i) res.mat[i][i] = 1;
        return res;
    }

    // Matrix multiplication optimized with loop reordering and a single modulo operation per row element
    Matrix operator*(const Matrix& other) const {
        Matrix res(sz);
        for (int i = 0; i < sz; ++i) {
            long long temp[32];
            memset(temp, 0, sizeof(temp));
            for (int k = 0; k < sz; ++k) {
                if (mat[i][k] == 0) continue;
                long long val = mat[i][k];
                for (int j = 0; j < sz; ++j) {
                    temp[j] += val * other.mat[k][j];
                }
            }
            for (int j = 0; j < sz; ++j) {
                res.mat[i][j] = temp[j] % P_val;
            }
        }
        return res;
    }
};

/**
 * Standard binary exponentiation for matrix power.
 */
Matrix matrix_pow(Matrix A, int p) {
    Matrix res = Matrix::identity(A.sz);
    while (p > 0) {
        if (p & 1) res = res * A;
        A = A * A;
        p >>= 1;
    }
    return res;
}

/**
 * Exponentiation for a large number N represented as a string.
 * Based on the principle that N = d_k*10^k + ... + d_0, so A^N = (...((A^{d_k})^{10} * A^{d_{k-1}})^{10} ...).
 */
Matrix large_pow(Matrix A, string N) {
    Matrix res = Matrix::identity(A.sz);
    for (char c : N) {
        // To compute res = res^10: res^10 = (((res^2)^2)^2) * res^2
        Matrix r2 = res * res;
        Matrix r4 = r2 * r2;
        Matrix r8 = r4 * r4;
        res = r8 * r2;

        int digit = c - '0';
        if (digit > 0) {
            res = res * matrix_pow(A, digit);
        }
    }
    return res;
}

/**
 * Subtracts one from a large number represented as a string.
 */
string subtractOne(string n) {
    int len = n.length();
    for (int i = len - 1; i >= 0; i--) {
        if (n[i] > '0') {
            n[i]--;
            break;
        } else {
            n[i] = '9';
        }
    }
    size_t first_nonzero = n.find_first_not_of('0');
    if (first_nonzero == string::npos) return "0";
    return n.substr(first_nonzero);
}

/**
 * Checks if two row configurations (masks) are compatible for a grid of width M.
 * Two rows are compatible if they do not form any 2x2 monochromatic square.
 */
bool is_compatible(int m1, int m2, int M) {
    for (int j = 0; j < M - 1; ++j) {
        int b1 = (m1 >> j) & 1;
        int b2 = (m1 >> (j + 1)) & 1;
        int b3 = (m2 >> j) & 1;
        int b4 = (m2 >> (j + 1)) & 1;
        if (b1 == b2 && b2 == b3 && b3 == b4) return false;
    }
    return true;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string N_str;
    int M, P;
    if (!(cin >> N_str >> M >> P)) return 0;
    P_val = P;

    // Special case for N=1 where no 2x2 squares can exist
    if (N_str == "1") {
        long long ans = 1;
        for (int i = 0; i < M; ++i) ans = (ans * 2) % P;
        cout << ans % P << endl;
        return 0;
    }

    int sz = 1 << M;
    Matrix A(sz);
    // Building the adjacency matrix A, where A[i][j] = 1 if row i can follow row j
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            if (is_compatible(i, j, M)) {
                A.mat[i][j] = 1;
            }
        }
    }

    // Calculating A^(N-1)
    string N_minus_1 = subtractOne(N_str);
    Matrix res = large_pow(A, N_minus_1);

    // The total number of nice patterns is the sum of all elements in the matrix A^(N-1)
    long long total_sum = 0;
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            total_sum = (total_sum + res.mat[i][j]) % P;
        }
    }

    cout << total_sum << endl;

    return 0;
}

