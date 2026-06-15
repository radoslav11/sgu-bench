#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<vector<ll>> Matrix;

int M;
ll P;
int num_states;
Matrix trans;

bool isValid(int row1, int row2) {
    // Check if two consecutive rows can be adjacent
    // row1 is above row2
    // Check all 2x2 squares: if all 4 bits are same, it's invalid
    for(int j = 0; j < M - 1; j++) {
        int b1 = (row1 >> j) & 1;
        int b2 = (row1 >> (j+1)) & 1;
        int b3 = (row2 >> j) & 1;
        int b4 = (row2 >> (j+1)) & 1;
        
        if(b1 == b2 && b2 == b3 && b3 == b4) {
            return false;
        }
    }
    return true;
}

Matrix multiply(const Matrix& a, const Matrix& b, ll mod) {
    int n = a.size();
    Matrix c(n, vector<ll>(n, 0));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % mod;
            }
        }
    }
    return c;
}

Matrix matpow(Matrix a, string n_str, ll mod) {
    int sz = a.size();
    Matrix result(sz, vector<ll>(sz, 0));
    for(int i = 0; i < sz; i++) result[i][i] = 1;
    
    // Process n_str from right to left (least significant first)
    while(n_str.length() > 0) {
        if((n_str.back() - '0') & 1) {
            result = multiply(result, a, mod);
        }
        a = multiply(a, a, mod);
        
        // Divide n_str by 2
        int carry = 0;
        for(int i = 0; i < n_str.length(); i++) {
            int digit = carry * 10 + (n_str[i] - '0');
            n_str[i] = '0' + (digit / 2);
            carry = digit % 2;
        }
        while(n_str.length() > 0 && n_str[0] == '0') {
            n_str.erase(0, 1);
        }
    }
    
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string N_str;
    cin >> N_str >> M >> P;
    
    num_states = 1 << M;
    trans.assign(num_states, vector<ll>(num_states, 0));
    
    // Build transition matrix
    for(int i = 0; i < num_states; i++) {
        for(int j = 0; j < num_states; j++) {
            if(isValid(i, j)) {
                trans[i][j] = 1;
            }
        }
    }
    
    // Compute trans^(N-1)
    string exp = N_str;
    // Subtract 1 from exp
    int borrow = 1;
    for(int i = exp.length() - 1; i >= 0 && borrow; i--) {
        int digit = (exp[i] - '0') - borrow;
        if(digit < 0) {
            exp[i] = '9';
            borrow = 1;
        } else {
            exp[i] = '0' + digit;
            borrow = 0;
        }
    }
    while(exp.length() > 0 && exp[0] == '0') {
        exp.erase(0, 1);
    }
    
    Matrix result;
    if(exp.empty() || exp == "0") {
        result.assign(num_states, vector<ll>(num_states, 0));
        for(int i = 0; i < num_states; i++) result[i][i] = 1;
    } else {
        result = matpow(trans, exp, P);
    }
    
    // Sum all entries
    ll ans = 0;
    for(int i = 0; i < num_states; i++) {
        for(int j = 0; j < num_states; j++) {
            ans = (ans + result[i][j]) % P;
        }
    }
    
    cout << ans << endl;
    
    return 0;
}
