#include<bits/stdc++.h>
using namespace std;

int N, M;
int wins[16][16];
map<int, long long> dp;

int minHeight(int n) {
    if (n == 1) return 0;
    return 1 + minHeight((n + 1) / 2);
}

long long solve(int mask) {
    int n = __builtin_popcount(mask);
    if (n == 1) {
        return ((mask >> M) & 1) ? 1 : 0;
    }
    
    if (dp.count(mask)) return dp[mask];
    
    int h = minHeight(n);
    long long ans = 0;
    
    for (int sub = (mask - 1) & mask; sub > 0; sub = (sub - 1) & mask) {
        int comp = mask ^ sub;
        int n1 = __builtin_popcount(sub);
        int n2 = __builtin_popcount(comp);
        
        if (minHeight(n1) + 1 == h && minHeight(n2) + 1 == h) {
            bool mInSub = (sub >> M) & 1;
            bool canWin = true;
            
            if (mInSub) {
                for (int i = 0; i < N; i++) {
                    if ((comp >> i) & 1 && !wins[M][i]) {
                        canWin = false;
                        break;
                    }
                }
            } else {
                for (int i = 0; i < N; i++) {
                    if ((sub >> i) & 1 && !wins[M][i]) {
                        canWin = false;
                        break;
                    }
                }
            }
            
            if (canWin) {
                ans += solve(sub) * solve(comp);
            }
        }
    }
    
    return dp[mask] = ans;
}

int main(){
    cin >> N >> M;
    M--;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> wins[i][j];
        }
    }
    
    cout << solve((1 << N) - 1) << endl;
    
    return 0;
}
