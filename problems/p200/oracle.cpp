#include <bits/stdc++.h>
using namespace std;

// Add two non-negative decimal strings a and b
string addStrings(const string &a, const string &b) {
    int i = a.size() - 1, j = b.size() - 1, carry = 0;
    string res;
    while (i >= 0 || j >= 0 || carry) {
        int da = (i >= 0 ? a[i--] - '0' : 0);
        int db = (j >= 0 ? b[j--] - '0' : 0);
        int sum = da + db + carry;
        res.push_back(char('0' + (sum % 10)));
        carry = sum / 10;
    }
    reverse(res.begin(), res.end());
    // remove leading zeros
    int pos = 0;
    while (pos + 1 < (int)res.size() && res[pos] == '0') pos++;
    return res.substr(pos);
}

// Subtract decimal string b from a (assuming a >= b >= "0")
string subStrings(const string &a, const string &b) {
    int i = a.size() - 1, j = b.size() - 1, borrow = 0;
    string res;
    while (i >= 0) {
        int da = a[i] - '0';
        int db = (j >= 0 ? b[j] - '0' : 0) + borrow;
        if (da < db) {
            da += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res.push_back(char('0' + (da - db)));
        i--; j--;
    }
    // trim and reverse
    while (res.size() > 1 && res.back() == '0') res.pop_back();
    reverse(res.begin(), res.end());
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int t, m;
    cin >> t >> m;

    vector<long long> b(m);
    for(int i = 0; i < m; i++) {
        cin >> b[i];
    }

    // 1) Generate the first t primes by simple trial division
    vector<int> primes;
    int candidate = 2;
    while ((int)primes.size() < t) {
        bool isP = true;
        for (int p : primes) {
            if ((long long)p * p > candidate) break;
            if (candidate % p == 0) {
                isP = false;
                break;
            }
        }
        if (isP) primes.push_back(candidate);
        candidate = (candidate == 2 ? 3 : candidate + 2);
    }

    // 2) Build the exponent-parity matrix over GF(2)
    //    Each row i is a vector of size t for b[i]
    vector<vector<int>> mat(m, vector<int>(t, 0));
    for (int i = 0; i < m; i++) {
        long long x = b[i];
        for (int j = 0; j < t; j++) {
            int p = primes[j];
            // Count how many times p divides x, mod 2
            while (x % p == 0) {
                mat[i][j] ^= 1;  // flip parity
                x /= p;
            }
            if (x == 1) break;
        }
        // By problem statement, after factoring all t primes, x must be 1
    }

    // 3) Gaussian elimination over GF(2) to compute rank r
    int rank = 0;
    for (int col = 0; col < t && rank < m; col++) {
        // Find a row >= rank with a 1 in this column
        int sel = -1;
        for (int row = rank; row < m; row++) {
            if (mat[row][col] == 1) {
                sel = row;
                break;
            }
        }
        if (sel == -1) continue; // no pivot in this column

        // Swap to bring pivot into 'rank'-th row
        swap(mat[rank], mat[sel]);

        // Eliminate this column from all other rows
        for (int row = 0; row < m; row++) {
            if (row != rank && mat[row][col] == 1) {
                // row_i ^= pivot_row
                for (int c = col; c < t; c++) {
                    mat[row][c] ^= mat[rank][c];
                }
            }
        }
        rank++;
    }

    // 4) Number of solutions = 2^(m - rank) - 1
    int freeVars = m - rank;
    // Compute power of two as decimal string by repeated doubling
    string ans = "1";
    for (int i = 0; i < freeVars; i++) {
        ans = addStrings(ans, ans);
    }
    // subtract 1 to exclude the empty subset
    ans = subStrings(ans, "1");

    cout << ans << "\n";
    return 0;
}
