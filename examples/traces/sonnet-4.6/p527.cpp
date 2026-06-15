#include<bits/stdc++.h>
using namespace std;
int match_l[30],match_r[30];
bool used[30];
vector<int>adj[30];
int n,m;
bool dfs(int u){
    for(int v:adj[u]){
        if(!used[v]){used[v]=1;
            if(match_r[v]==-1||dfs(match_r[v])){match_l[u]=v;match_r[v]=u;return 1;}
        }
    }
    return 0;
}
int main(){
    cin>>n>>m;
    vector<string>g(n);
    for(auto&s:g)cin>>s;
    memset(match_l,-1,sizeof(match_l));
    memset(match_r,-1,sizeof(match_r));
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)if(g[i][j]=='*')adj[i].push_back(j);
    int res=0;
    for(int i=0;i<n;i++){memset(used,0,sizeof(used));if(dfs(i))res++;}
    cout<<res<<endl;
}
