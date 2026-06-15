#include <bits/stdc++.h>
using namespace std;

int main(){
    int K,N;
    scanf("%d %d",&K,&N);
    vector<pair<long long,int>> greens, blacks; // (cost,index)
    for(int i=1;i<=N;i++){
        long long c; int s;
        scanf("%lld %d",&c,&s);
        if(s==0) greens.push_back({c,i});
        else blacks.push_back({c,i});
    }
    sort(greens.begin(),greens.end());
    sort(blacks.begin(),blacks.end());
    int gav=greens.size(), bav=blacks.size();
    
    const long long INF=LLONG_MAX/4;
    // dp[i][g][s], s: 0=G1,1=G2,2=B1,3=B2
    long long *dp = new long long[(long long)(K+1)*(K+1)*4];
    char *mv = new char[(long long)(K+1)*(K+1)*4];
    char *ps = new char[(long long)(K+1)*(K+1)*4];
    auto IDX=[&](int i,int g,int s)->long long{
        return (((long long)i*(K+1)+g)*4+s);
    };
    long long total=(long long)(K+1)*(K+1)*4;
    for(long long t=0;t<total;t++) dp[t]=INF;
    
    int w1=K;
    if(gav>=1){
        long long id=IDX(1,1,0);
        dp[id]=(long long)w1*greens[0].first; mv[id]=0; ps[id]=4;
    }
    if(bav>=1){
        long long id=IDX(1,0,2);
        dp[id]=(long long)w1*blacks[0].first; mv[id]=1; ps[id]=4;
    }
    
    for(int i=2;i<=K;i++){
        int w=K-i+1;
        for(int g=0;g<=i-1;g++){
            if(g>gav) break;
            for(int s=0;s<4;s++){
                long long cur=dp[IDX(i-1,g,s)];
                if(cur>=INF) continue;
                // play green
                if(s!=1){
                    int ng=g+1;
                    if(ng<=gav){
                        int news=(s==0)?1:0;
                        long long cost=cur+(long long)w*greens[g].first;
                        long long id=IDX(i,ng,news);
                        if(cost<dp[id]){ dp[id]=cost; mv[id]=0; ps[id]=(char)s; }
                    }
                }
                // play black
                if(s!=3){
                    int blackBefore=(i-1)-g;
                    int nb=blackBefore+1;
                    if(nb<=bav){
                        int news=(s==2)?3:2;
                        long long cost=cur+(long long)w*blacks[blackBefore].first;
                        long long id=IDX(i,g,news);
                        if(cost<dp[id]){ dp[id]=cost; mv[id]=1; ps[id]=(char)s; }
                    }
                }
            }
        }
    }
    
    // find best at i=K
    long long best=INF; int bg=-1, bs=-1;
    for(int g=0;g<=K;g++){
        if(g>gav) break;
        for(int s=0;s<4;s++){
            long long v=dp[IDX(K,g,s)];
            if(v<best){ best=v; bg=g; bs=s; }
        }
    }
    
    if(best>=INF){
        printf("Impossible\n");
        delete[] dp; delete[] mv; delete[] ps;
        return 0;
    }
    
    // reconstruct colors
    vector<int> colors(K+1);
    int i=K, g=bg, s=bs;
    while(i>=1){
        long long id=IDX(i,g,s);
        int m=mv[id];
        int prev=ps[id];
        colors[i]=m;
        if(m==0) g-=1;
        s=prev;
        i--;
    }
    
    // forward assign
    int gc=0, bc=0;
    vector<int> ans(K);
    for(int r=1;r<=K;r++){
        if(colors[r]==0){ ans[r-1]=greens[gc].second; gc++; }
        else { ans[r-1]=blacks[bc].second; bc++; }
    }
    for(int r=0;r<K;r++){
        printf("%d%c", ans[r], r+1<K?' ':'\n');
    }
    
    delete[] dp; delete[] mv; delete[] ps;
    return 0;
}
