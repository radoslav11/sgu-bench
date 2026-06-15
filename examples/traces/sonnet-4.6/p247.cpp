#include <bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;

typedef vector<long long> BigInt;
// store in base 10^9, little-endian

const long long BASE = 1000000000LL;

BigInt fromLL(long long v) {
    BigInt r;
    if (v == 0) r.push_back(0);
    while (v > 0) { r.push_back(v % BASE); v /= BASE; }
    return r;
}

BigInt add(const BigInt& a, const BigInt& b) {
    BigInt r;
    long long carry = 0;
    for (int i = 0; i < (int)max(a.size(), b.size()) || carry; i++) {
        long long s = carry;
        if (i < (int)a.size()) s += a[i];
        if (i < (int)b.size()) s += b[i];
        r.push_back(s % BASE);
        carry = s / BASE;
    }
    return r;
}

BigInt mul(const BigInt& a, long long b) {
    BigInt r;
    long long carry = 0;
    for (int i = 0; i < (int)a.size() || carry; i++) {
        long long s = carry;
        if (i < (int)a.size()) s += a[i] * b;
        r.push_back(s % BASE);
        carry = s / BASE;
    }
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    return r;
}

string toString(const BigInt& a) {
    string s = to_string(a.back());
    for (int i = (int)a.size()-2; i >= 0; i--) {
        string t = to_string(a[i]);
        s += string(9 - t.size(), '0') + t;
    }
    return s;
}

int main(){
    int N;
    cin >> N;
    while (N--) {
        int P;
        cin >> P;
        // dp[j][k] = BigInt, j = number of balls chosen so far, k = sum mod P
        // Process P residues (0..P-1)
        // Use 2D array of BigInt
        vector<vector<BigInt>> dp(P+1, vector<BigInt>(P, fromLL(0)));
        dp[0][0] = fromLL(1);
        
        for (int r = 0; r < P; r++) {
            // Process in reverse to avoid using same residue twice? No, each residue is independent.
            // We need to create new dp each step
            vector<vector<BigInt>> ndp(P+1, vector<BigInt>(P, fromLL(0)));
            for (int j = 0; j <= P; j++) {
                for (int k = 0; k < P; k++) {
                    // check if dp[j][k] is nonzero
                    bool nonzero = false;
                    for (auto x : dp[j][k]) if (x) { nonzero = true; break; }
                    if (!nonzero) continue;
                    // x_r = 0
                    ndp[j][k] = add(ndp[j][k], dp[j][k]);
                    // x_r = 1
                    if (j+1 <= P) {
                        int nk = (k + r) % P;
                        ndp[j+1][nk] = add(ndp[j+1][nk], mul(dp[j][k], 2));
                    }
                    // x_r = 2
                    if (j+2 <= P) {
                        int nk = (k + 2*r) % P;
                        ndp[j+2][nk] = add(ndp[j+2][nk], dp[j][k]);
                    }
                }
            }
            dp = ndp;
        }
        
        cout << toString(dp[P][0]) << "\n";
    }
}
