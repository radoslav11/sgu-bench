#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    int total=N*N;
    vector<bool> alive(total+1,true);
    // BFS reachability in exactly K steps from alive set
    int dx[]={0,0,1,-1},dy[]={1,-1,0,0};
    auto id=[&](int r,int c){return (r-1)*N+c;};
    set<int> usedK;
    while(true){
        int cnt=count(alive.begin()+1,alive.end(),true);
        if(cnt==1) break;
        for(int K=N;K<300;K++){
            if(usedK.count(K)) continue;
            // BFS: reachable in exactly K steps from alive positions
            vector<vector<int>> dp(K+1,vector<int>(total+1,0));
            for(int i=1;i<=total;i++) if(alive[i]) dp[0][i]=1;
            for(int s=0;s<K;s++) for(int i=1;i<=N;i++) for(int j=1;j<=N;j++){int u=id(i,j);if(!dp[s][u])continue;for(int d=0;d<4;d++){int ni=i+dx[d],nj=j+dy[d];if(ni>=1&&ni<=N&&nj>=1&&nj<=N)dp[s+1][id(ni,nj)]=1;}}
            vector<int> rem;
            for(int i=1;i<=total;i++) if(alive[i]&&!dp[K][i]) rem.push_back(i);
            if(!rem.empty()){
                usedK.insert(K);
                cout<<K;
                for(int x:rem){cout<<" "<<x;alive[x]=false;}
                cout<<"\n";
                break;
            }
        }
    }
}
