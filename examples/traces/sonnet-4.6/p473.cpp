#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
typedef __int128 lll;
int primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};
int K;
lll best;
void dfs(int rem, int maxFactor, int idx, lll cur) {
    if (rem == 1) { if (cur < best) best = cur; return; }
    if (idx >= 17) return;
    for (int f = 2; f <= min(rem, maxFactor); f++) {
        if (rem % f == 0) {
            lll pw = 1; bool ov = false;
            for (int i = 0; i < f-1; i++) { pw *= primes[idx]; if (pw > best/cur) { ov=true; break; } }
            if (!ov) dfs(rem/f, f, idx+1, cur*pw);
        }
    }
}
int main(){
    cin >> K;
    best = (lll)4e18;
    dfs(K, K, 0, 1);
    if (best >= (lll)4e18) cout << 0;
    else { ull ans = (ull)best; cout << ans; }
}
