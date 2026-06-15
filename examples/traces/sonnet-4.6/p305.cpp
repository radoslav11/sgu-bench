#include <bits/stdc++.h>
using namespace std;

// Hopcroft-Karp
struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> matchL, matchR, dist;
    HopcroftKarp(int n, int m) : n(n), m(m), adj(n), matchL(n,-1), matchR(m,-1), dist(n) {}
    void addEdge(int u, int v) { adj[u].push_back(v); }
    bool bfs() {
        queue<int> q;
        for(int u=0;u<n;u++) { if(matchL[u]==-1){dist[u]=0;q.push(u);}else dist[u]=1e9;}
        bool found=false;
        while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u]){int w=matchR[v];if(w==-1)found=true;else if(dist[w]>1e8){dist[w]=dist[u]+1;q.push(w);}}}
        return found;
    }
    bool dfs(int u){for(int v:adj[u]){int w=matchR[v];if(w==-1||dist[w]==dist[u]+1&&dfs(w)){matchL[u]=v;matchR[v]=u;return true;}}dist[u]=1e9;return false;}
    int maxMatch(){int r=0;while(bfs())for(int u=0;u<n;u++)if(matchL[u]==-1)r+=dfs(u);return r;}
};

int main(){
    int N; long long M;
    cin>>N>>M;
    vector<long long> A(N),B(N);
    for(int i=0;i<N;i++) cin>>A[i]>>B[i];
    vector<long long> cands;
    for(int i=0;i<N;i++){
        long long a=A[i],b=B[i];
        if(a==0){if(b>=1&&b<=M)cands.push_back(b);}
        else if(a>0){long long k=max(0LL,(long long)ceil((1.0-b)/a));int cnt=0;while(cnt<=N){long long x=b+k*a;if(x>M)break;if(x>=1){cands.push_back(x);cnt++;}k++;}}
        else{long long k=max(0LL,(long long)ceil((b-M)/(-a)));int cnt=0;while(cnt<=N){long long x=b+k*a;if(x<1)break;if(x<=M){cands.push_back(x);cnt++;}k++;}}
    }
    sort(cands.begin(),cands.end());cands.erase(unique(cands.begin(),cands.end()),cands.end());
    int C=cands.size();
    HopcroftKarp hk(N,C);
    for(int i=0;i<N;i++){long long a=A[i],b=B[i];for(int j=0;j<C;j++){long long x=cands[j];if(a==0){if(x==b)hk.addEdge(i,j);}else{long long d=x-b;if(d%a==0&&d/a>=0)hk.addEdge(i,j);}}}
    hk.maxMatch();
    // assign unmatched a number not in cands
    set<long long> used(cands.begin(),cands.end());
    vector<long long> ans(N);
    for(int i=0;i<N;i++) ans[i]=(hk.matchL[i]==-1)?-1:cands[hk.matchL[i]];
    long long nxt=1;
    for(int i=0;i<N;i++){if(ans[i]==-1){while(used.count(nxt))nxt++;ans[i]=nxt;used.insert(nxt);nxt++;}}
    for(int i=0;i<N;i++) cout<<ans[i]<<" \n"[i==N-1];
}
