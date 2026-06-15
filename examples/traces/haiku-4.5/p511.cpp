#include<bits/stdc++.h>
using namespace std;

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
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
    
    int t;
    cin >> t;
    
    while(t--) {
        long long n, p;
        cin >> n >> p;
        
        // Precompute all n-th power residues
        map<long long, long long> nth_powers;
        for(long long z = 1; z < p; z++) {
            long long power = mod_pow(z, n, p);
            if(nth_powers.find(power) == nth_powers.end()) {
                nth_powers[power] = z;
            }
        }
        
        bool found = false;
        long long ans_x, ans_y, ans_z;
        
        // Try pairs with a reasonable limit
        long long limit = min(p, 10000LL);
        for(long long x = 1; x < limit && !found; x++) {
            for(long long y = 1; y < limit && !found; y++) {
                long long xn = mod_pow(x, n, p);
                long long yn = mod_pow(y, n, p);
                long long sum = (xn + yn) % p;
                
                if(nth_powers.find(sum) != nth_powers.end()) {
                    ans_x = x;
                    ans_y = y;
                    ans_z = nth_powers[sum];
                    found = true;
                }
            }
        }
        
        if(found) {
            cout << ans_x << " " << ans_y << " " << ans_z << "\n";
        } else {
            cout << "-1\n";
        }
    }
    
    return 0;
}
