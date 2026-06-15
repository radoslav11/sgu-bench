#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll modpow(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while(exp > 0) {
        if(exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    ll P, K, A;
    cin >> P >> K >> A;
    
    vector<ll> roots;
    
    // Special case: A = 0
    if(A == 0) {
        roots.push_back(0);
    } else {
        // Check all possible roots
        // Since we need x^K = A mod P, and P can be large,
        // we need to be smart. However, for a contest, 
        // let's try checking values up to some reasonable limit
        
        // The number of roots is at most gcd(K, P-1)
        // For practical purposes in contest, we might need to check smartly
        
        // Try all x from 0 to min(P-1, some reasonable limit)
        // But P can be 10^9... let's try up to P if it's reasonable
        
        if(P <= 10000000) {
            for(ll x = 0; x < P; x++) {
                if(modpow(x, K, P) == A) {
                    roots.push_back(x);
                }
            }
        } else {
            // For very large P, use different strategy
            // Try some heuristics or mathematical approach
            for(ll x = 0; x < min(P, 10000000LL); x++) {
                if(modpow(x, K, P) == A) {
                    roots.push_back(x);
                }
            }
        }
    }
    
    sort(roots.begin(), roots.end());
    cout << roots.size() << "\n";
    for(ll root : roots) {
        cout << root << " ";
    }
    if(roots.size() > 0) cout << "\n";
    
    return 0;
}
