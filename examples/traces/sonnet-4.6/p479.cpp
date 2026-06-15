#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    vector<vector<int>> val(n+1, vector<int>(m+1));
    for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) cin>>val[i][j];
    
    int dx[]={0,0,1,-1}, dy[]={1,-1,0,0};
    vector<vector<int>> need(n+1, vector<int>(m+1,0));
    bool possible = true;
    for(int i=1;i<=n;i++) for(int j=1;j<=m;j++){
        int deg=0;
        for(int d=0;d<4;d++){int ni=i+dx[d],nj=j+dy[d]; if(ni>=1&&ni<=n&&nj>=1&&nj<=m) deg++;}
        need[i][j] = deg - (val[i][j]-1);
        if(need[i][j]<0||need[i][j]>deg) possible=false;
    }
    if(!possible){cout<<"No solution\n";return 0;}
    
    queue<pair<int,int>> q;
    for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) if(need[i][j]==0) q.push({i,j});
    
    vector<pair<int,int>> order;
    while(!q.empty()){
        auto [r,c]=q.front(); q.pop();
        order.push_back({r,c});
        for(int d=0;d<4;d++){int nr=r+dx[d],nc=c+dy[d]; if(nr>=1&&nr<=n&&nc>=1&&nc<=m){need[nr][nc]--;if(need[nr][nc]==0)q.push({nr,nc});}}
    }
    if((int)order.size()!=n*m){cout<<"No solution\n";return 0;}
    for(auto [r,c]:order) cout<<r<<" "<<c<<"\n";
}
