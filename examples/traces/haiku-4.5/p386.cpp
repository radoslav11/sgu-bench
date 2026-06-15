#include<bits/stdc++.h>
using namespace long long;

typedef long long ll;

ll mod;

ll modpow(ll base, ll exp, ll p) {
    ll result = 1;
    base %= p;
    while(exp > 0) {
        if(exp % 2 == 1) result = (result * base) % p;
        base = (base * base) % p;
        exp /= 2;
    }
    return result;
}

ll modinv(ll a, ll p) {
    return modpow(a, p-2, p);
}

ll determinant(vector<vector<ll>>& matrix, int size) {
    ll det = 1;
    vector<vector<ll>> m = matrix;
    
    for(int i = 0; i < size; i++) {
        int pivot = i;
        for(int j = i+1; j < size; j++) {
            if(abs(m[j][i]) > abs(m[pivot][i])) pivot = j;
        }
        if(m[pivot][i] == 0) return 0;
        
        if(pivot != i) {
            swap(m[i], m[pivot]);
            det = (mod - det) % mod;
        }
        
        det = (det * m[i][i]) % mod;
        ll inv = modinv(m[i][i], mod);
        
        for(int j = i; j < size; j++) {
            m[i][j] = (m[i][j] * inv) % mod;
        }
        
        for(int j = i+1; j < size; j++) {
            ll factor = m[j][i];
            for(int k = i; k < size; k++) {
                m[j][k] = (m[j][k] - (factor * m[i][k]) % mod + mod) % mod;
            }
        }
    }
    return det;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n >> mod;
    
    vector<vector<ll>> v(n, vector<ll>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> v[i][j];
            v[i][j] %= mod;
        }
    }
    
    ll answer = 0;
    
    for(int mask = 1; mask < (1 << n); mask++) {
        int k = __builtin_popcount(mask);
        vector<vector<ll>> sub;
        int idx = 0;
        for(int i = 0; i < n; i++) {
            if(mask & (1 << i)) {
                sub.push_back(v[i]);
            }
        }
        
        ll det = determinant(sub, k);
        ll contrib = (modpow(2, k, mod) * det) % mod;
        answer = (answer + contrib) % mod;
    }
    
    cout << answer << endl;
    
    return 0;
}
