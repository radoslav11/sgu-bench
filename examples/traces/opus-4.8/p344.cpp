#include <bits/stdc++.h>
using namespace std;

int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    vector<string> g(n);
    for(int i=0;i<n;i++){
        char buf[1005];
        scanf("%s",buf);
        g[i]=buf;
    }
    vector<vector<int>> occ(n, vector<int>(m,0));
    queue<pair<int,int>> q;
    int cnt=0;
    for(int i=0;i<n;i++)for(int j=0;j<m;j++){
        if(g[i][j]=='X'){occ[i][j]=1;cnt++;q.push({i,j});}
    }
    int dx[]={-1,1,0,0};
    int dy[]={0,0,-1,1};
    while(!q.empty()){
        auto [x,y]=q.front();q.pop();
        // check neighbors that are free, see if they become occupied
        for(int d=0;d<4;d++){
            int nx=x+dx[d], ny=y+dy[d];
            if(nx<0||nx>=n||ny<0||ny>=m)continue;
            if(occ[nx][ny])continue;
            // count occupied neighbors of (nx,ny)
            int c=0;
            for(int dd=0;dd<4;dd++){
                int ax=nx+dx[dd], ay=ny+dy[dd];
                if(ax<0||ax>=n||ay<0||ay>=m)continue;
                if(occ[ax][ay])c++;
            }
            if(c>=2){
                occ[nx][ny]=1;cnt++;q.push({nx,ny});
            }
        }
    }
    printf("%d\n",cnt);
    return 0;
}
