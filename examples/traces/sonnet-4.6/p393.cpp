#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    cin >> N >> M;
    
    vector<pair<int,int>> words(M);
    for(int i=0;i<M;i++){
        string s; cin>>s;
        words[i] = {s[0]-'a', s[1]-'a'};
    }
    
    // Build conflict graph
    vector<int> adj(N, 0);
    for(int i=0;i<N;i++)
        for(int j=i+1;j<N;j++){
            bool conflict = false;
            for(auto& w: words){
                // check if i,j share right neighbor
                // words (i,x) and (j,x)
                // words (x,i) and (x,j)
            }
            for(auto& w1: words){
                for(auto& w2: words){
                    if(w1==w2) continue;
                    if((w1.first==i && w2.first==j && w1.second==w2.second) ||
                       (w1.second==i && w2.second==j && w1.first==w2.first))
                        conflict=true;
                }
            }
            if(conflict){
                adj[i]|=(1<<j);
                adj[j]|=(1<<i);
            }
        }
    
    int full = (1<<N)-1;
    // Precompute independent sets
    vector<bool> indep(1<<N, true);
    for(int mask=1;mask<=(1<<N)-1;mask++){
        int v = __builtin_ctz(mask);
        if((adj[v]&mask)!=0) indep[mask]=false;
        else indep[mask]=indep[mask^(1<<v)];
    }
    
    vector<int> dp(1<<N, INT_MAX);
    vector<int> par(1<<N, -1);
    dp[0]=0;
    for(int mask=1;mask<=(1<<N)-1;mask++){
        for(int sub=mask;sub>0;sub=(sub-1)&mask){
            if(indep[sub] && dp[mask^sub]!=INT_MAX && dp[mask^sub]+1<dp[mask]){
                dp[mask]=dp[mask^sub]+1;
                par[mask]=sub;
            }
        }
    }
    
    cout << dp[full] << "\n";
    int mask=full;
    while(mask){
        int s=par[mask];
        for(int i=0;i<N;i++) if(s>>i&1) cout<<(char)('a'+i);
        cout<<"\n";
        mask^=s;
    }
}
