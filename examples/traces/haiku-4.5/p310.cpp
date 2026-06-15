#include <bits/stdc++.h>
using namespace std;

int n, m, k;
map<pair<int, long long>, long long> memo;

long long dp(int pos, long long lastBits) {
    if(pos == n) {
        return 1;
    }
    
    auto state = make_pair(pos, lastBits);
    if(memo.count(state)) {
        return memo[state];
    }
    
    long long result = 0;
    
    for(int bit = 0; bit <= 1; bit++) {
        long long newBits = ((lastBits << 1) | bit) & ((1LL << (m-1)) - 1);
        
        // Check constraint: if we have at least m bits, check the last m bits
        if(pos >= m - 1) {
            long long window = (lastBits << 1) | bit;
            int ones = __builtin_popcountll(window & ((1LL << m) - 1));
            if(ones >= k) {
                result += dp(pos + 1, newBits);
            }
        } else {
            result += dp(pos + 1, newBits);
        }
    }
    
    return memo[state] = result;
}

int main(){
    cin >> n >> m >> k;
    cout << dp(0, 0) << endl;
    return 0;
}
