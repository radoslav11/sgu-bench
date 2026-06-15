#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

ll mod_pow(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main(){
    int N, M;
    ll P;
    cin >> N >> M >> P;
    
    vector<vector<int>> partitions;
    function<void(int, int, vector<int>&)> gen = [&](int n, int m, vector<int>& v) {
        if (n == 0) { partitions.push_back(v); return; }
        for (int i = min(n, m); i >= 1; i--) {
            v.push_back(i);
            gen(n - i, i, v);
            v.pop_back();
        }
    };
    vector<int> v;
    gen(N, N, v);
    
    ll ans = 0;
    ll fact_N = 1;
    for (int i = 1; i <= N; i++) fact_N = (fact_N * i) % P;
    
    for (auto& part : partitions) {
        ll cnt = 1;
        map<int, int> freq;
        for (int x : part) freq[x]++;
        
        for (auto [len, f] : freq) {
            for (int i = 0; i < f; i++) cnt = (cnt * len) % P;
            ll f_fact = 1;
            for (int i = 1; i <= f; i++) f_fact = (f_fact * i) % P;
            cnt = (cnt * mod_pow(f_fact, P-2, P)) % P;
        }
        
        ll orbits = 0;
        for (int c : part) orbits += (ll)c * (c-1) / 2;
        for (int i = 0; i < part.size(); i++) 
            for (int j = i+1; j < part.size(); j++)
                orbits += (ll)part[i] * part[j];
        
        ans = (ans + cnt * mod_pow(M, orbits, P)) % P;
    }
    
    ans = (ans * mod_pow(fact_N, P-2, P)) % P;
    cout << ans << endl;
    return 0;
}
