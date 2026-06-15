#include <bits/stdc++.h>
using namespace std;
int M,N,grid[205][205],id[205][205];
vector<int> adj[40005];
int matchL[40005],matchR[40005],nL,nR;
bool used[40005];
bool dfs(int u){for(int v:adj[u])if(!used[v]){used[v]=true;if(matchR[v]==-1||dfs(matchR[v])){matchL[u]=v;matchR[v]=u;return true;}}return false;}
int dx[]={0,0,1,-1},dy[]={1,-1,0,0};
int main(){
    ios_base::sync_with_stdio(false);cin.tie(NULL);
    cin>>M>>N;
    for(int i=0;i<M;i++)for(int j=0;j<N;j++)cin>>grid[i][j];
    nL=nR=0;memset(id,-1,sizeof(id));
    for(int i=0;i<M;i++)for(int j=0;j<N;j++)if(grid[i][j]==1){if((i+j)%2==0)id[i][j]=nL++;else id[i][j]=nR++;}
    for(int i=0;i<M;i++)for(int j=0;j<N;j++)if(grid[i][j]==1&&(i+j)%2==0)
        for(int d=0;d<4;d++){int ni=i+dx[d],nj=j+dy[d];if(ni>=0&&ni<M&&nj>=0&&nj<N&&grid[ni][nj]==1)adj[id[i][j]].push_back(id[ni][nj]);}
    memset(matchL,-1,sizeof(matchL));memset(matchR,-1,sizeof(matchR));
    int matching=0;
    for(int u=0;u<nL;u++){memset(used,false,sizeof(bool)*nR);if(dfs(u))matching++;}
    vector<bool> inZL(nL,false),inZR(nR,false);
    queue<int> q;
    for(int u=0;u<nL;u++)if(matchL[u]==-1){inZL[u]=true;q.push(u);}
    while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u])if(!inZR[v]){inZR[v]=true;if(matchR[v]!=-1&&!inZL[matchR[v]]){inZL[matchR[v]]=true;q.push(matchR[v]);}}}
    int total=0;for(int i=0;i<M;i++)for(int j=0;j<N;j++)if(grid[i][j]==1)total++;
    cout<<total-matching<<"\n";
    for(int i=0;i<M;i++){for(int j=0;j<N;j++){if(!grid[i][j])cout<<'#';else{bool inMIS=((i+j)%2==0)?inZL[id[i][j]]:!inZR[id[i][j]];cout<<(inMIS?'G':'.');}}cout<<"\n";}
}
