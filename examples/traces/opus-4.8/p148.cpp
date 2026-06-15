#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    if(scanf("%d",&N)!=1) return 0;
    vector<long long> W(N+1), L(N+1), P(N+1), pref(N+1,0), thr(N+1);
    for(int i=1;i<=N;i++){
        long long w,l,p;
        scanf("%lld %lld %lld",&w,&l,&p);
        W[i]=w; L[i]=l; P[i]=p;
    }
    for(int i=1;i<=N;i++) pref[i]=pref[i-1]+W[i];
    // threshold[k] = pref[k]-L[k]; level k pays iff base >= thr[k] (base = pref[j-1])
    for(int i=1;i<=N;i++) thr[i]=pref[i]-L[i];
    
    long long best = LLONG_MAX;
    int bestj = -1;
    for(int j=1;j<=N;j++){
        long long base = pref[j-1];
        long long cost=0;
        for(int k=j;k<=N;k++){
            // water at k = pref[k]-base ; pay if <= L[k]  <=> base >= thr[k]
            if(thr[k] <= base) cost += P[k];
        }
        if(cost < best){
            best = cost;
            bestj = j;
            if(best==0) break; // can't do better
        }
    }
    
    long long base = pref[bestj-1];
    for(int k=bestj;k<=N;k++){
        if(thr[k] <= base) printf("%d\n", k);
    }
    return 0;
}
