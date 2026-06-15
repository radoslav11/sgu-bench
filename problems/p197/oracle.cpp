
#include <bits/stdc++.h>
using namespace std;

// Multiply two S×S matrices A and B modulo mod
vector<vector<int>> matMul(const vector<vector<int>>& A,
                           const vector<vector<int>>& B,
                           int mod) {
    int S = A.size();
    vector<vector<int>> C(S, vector<int>(S, 0));
    for(int i = 0; i < S; i++) {
        for(int k = 0; k < S; k++) if(A[i][k]) {
            int a = A[i][k];
            for(int j = 0; j < S; j++) {
                C[i][j] = (C[i][j] + a * B[k][j]) % mod;
            }
        }
    }
    return C;
}

// Multiply S×S matrix A by length‐S vector v modulo mod
vector<int> matVec(const vector<vector<int>>& A,
                   const vector<int>& v,
                   int mod) {
    int S = A.size();
    vector<int> res(S, 0);
    for(int i = 0; i < S; i++) {
        long long sum = 0;
        for(int j = 0; j < S; j++) {
            sum += 1LL * A[i][j] * v[j];
        }
        res[i] = sum % mod;
    }
    return res;
}

// Subtract 1 from a decimal string (N >= 1)
void decMinusOne(string &s) {
    int i = s.size() - 1;
    while(i >= 0) {
        if(s[i] > '0') { s[i]--; break; }
        s[i] = '9';
        i--;
    }
    // remove leading zero if any
    if(s[0] == '0' && s.size() > 1) s.erase(s.begin());
}

// Divide decimal string by 2, return remainder (0 or 1)
int div2(string &s) {
    int carry = 0;
    for(char &c : s) {
        int x = carry * 10 + (c - '0');
        c = char('0' + x/2);
        carry = x % 2;
    }
    // strip leading zeros
    if(s[0]=='0' && s.size()>1) s.erase(s.begin());
    return carry;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string Nstr;
    int M, P;
    cin >> Nstr >> M >> P;

    // State count S = 2^M
    int S = 1 << M;
    // Build transition matrix T[prev][cur]
    vector<vector<int>> T(S, vector<int>(S, 0));
    for(int prev = 0; prev < S; prev++){
        for(int cur = 0; cur < S; cur++){
            bool ok = true;
            // check every adjacent column pair in 2×2 block
            for(int k = 1; k < M; k++){
                int a = (prev >> (k-1)) & 1;
                int b = (prev >> k) & 1;
                int c = (cur  >> (k-1)) & 1;
                int d = (cur  >> k) & 1;
                // if all four equal 0 or all four equal 1 → invalid
                if((a+b+c+d) == 0 || (a+b+c+d) == 4){
                    ok = false;
                    break;
                }
            }
            if(ok) T[prev][cur] = 1;
        }
    }

    // Initial vector V1: all masks are allowed in first row
    vector<int> V(S, 1);

    // We need T^(N-1). Compute exponent = Nstr - 1
    if(Nstr == "0"){
        // no rows, only 1 way? but N>=1 by problem
    }
    decMinusOne(Nstr);  // now Nstr = N-1

    // Initialize result matrix R = identity
    vector<vector<int>> R(S, vector<int>(S, 0));
    for(int i = 0; i < S; i++) R[i][i] = 1;

    // Fast exponentiation of T to power N-1
    while(!(Nstr.size()==1 && Nstr[0]=='0')){
        int bit = div2(Nstr);  // bit = old Nstr % 2
        if(bit == 1){
            R = matMul(R, T, P);
        }
        T = matMul(T, T, P);
    }

    // Multiply R by initial vector V to get V_N
    vector<int> VN = matVec(R, V, P);

    // Sum all entries to get total patterns modulo P
    int answer = 0;
    for(int x : VN){
        answer = (answer + x) % P;
    }
    cout << answer << "\n";
    return 0;
}
