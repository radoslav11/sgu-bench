#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    vector<int> w(n), t(n), s(n);
    for(int i=0;i<n;i++) scanf("%d %d %d", &w[i], &t[i], &s[i]);
    
    int full = 1<<n;
    // weight sum per mask
    vector<int> sw(full,0), maxWalker(full,0), sOn(full,0);
    for(int mask=0; mask<full; mask++){
        int wt=0, mw=0, so=0;
        for(int j=0;j<n;j++){
            if(mask&(1<<j)){ wt+=w[j]; so+=s[j]; }
            else { if(t[j]>mw) mw=t[j]; }
        }
        sw[mask]=wt; maxWalker[mask]=mw; sOn[mask]=so;
    }
    
    // changeCost[T][T'] = sum of s_j where bit differs
    // flatten: changeCost[T*full+Tp]
    vector<int> changeCost((size_t)full*full, 0);
    for(int T=0;T<full;T++){
        for(int Tp=0;Tp<full;Tp++){
            int diff = T^Tp;
            int c=0;
            // compute sum of s over set bits of diff
            int d=diff;
            while(d){
                int b = d & (-d);
                int idx = __builtin_ctz(b);
                c += s[idx];
                d ^= b;
            }
            changeCost[(size_t)T*full+Tp]=c;
        }
    }
    
    const long long INF = LLONG_MAX/4;
    vector<long long> dp(full, INF), ndp(full, INF);
    
    // riffle data
    // process segment by segment
    // initial: segment 1 (index 0)
    // moveTime(i,mask): if sw[mask]>c -> D else d; result max(segTime, maxWalker[mask])
    
    for(int i=0;i<m;i++){
        int c,D,d;
        scanf("%d %d %d", &c, &D, &d);
        // compute moveTime per mask (non-empty)
        // for segment i
        if(i==0){
            for(int mask=1; mask<full; mask++){
                int seg = (sw[mask] > c) ? D : d;
                int mv = max(seg, maxWalker[mask]);
                dp[mask] = (long long)sOn[mask] + mv;
            }
        } else {
            // compute moveTime for this segment per mask
            // ndp[Tp] = min over T (dp[T] + change(T,Tp)) + moveTime(Tp)
            for(int mask=1; mask<full; mask++) ndp[mask]=INF;
            for(int T=1; T<full; T++){
                long long base = dp[T];
                if(base>=INF) continue;
                const int* cc = &changeCost[(size_t)T*full];
                for(int Tp=1; Tp<full; Tp++){
                    long long val = base + cc[Tp];
                    if(val < ndp[Tp]) ndp[Tp]=val;
                }
            }
            for(int mask=1; mask<full; mask++){
                if(ndp[mask]>=INF) continue;
                int seg = (sw[mask] > c) ? D : d;
                int mv = max(seg, maxWalker[mask]);
                ndp[mask] += mv;
            }
            dp.swap(ndp);
        }
    }
    
    long long ans = INF;
    for(int mask=1; mask<full; mask++){
        if(dp[mask]>=INF) continue;
        long long v = dp[mask] + sOn[mask]; // everyone off at finish
        if(v<ans) ans=v;
    }
    printf("%lld\n", ans);
    return 0;
}
