#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<string> grid(N);
    for(int i = 0; i < N; i++) cin >> grid[i];
    
    vector<vector<int>> region(N, vector<int>(M, -1));
    vector<int> regionSize;
    int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
    int numRegions = 0;
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(region[i][j] == -1){
                queue<pair<int,int>> q;
                q.push({i,j});
                region[i][j] = numRegions;
                int sz = 0;
                while(!q.empty()){
                    auto [x,y] = q.front(); q.pop(); sz++;
                    for(int d=0;d<4;d++){
                        int nx=x+dx[d], ny=y+dy[d];
                        if(nx>=0&&nx<N&&ny>=0&&ny<M&&region[nx][ny]==-1&&grid[nx][ny]==grid[x][y]){
                            region[nx][ny]=numRegions;
                            q.push({nx,ny});
                        }
                    }
                }
                regionSize.push_back(sz);
                numRegions++;
            }
    
    vector<bool> bigRegion(numRegions, false);
    for(int i=0;i<numRegions;i++) if(regionSize[i]>=K) bigRegion[i]=true;
    
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    queue<pair<int,int>> q;
    for(int i=0;i<N;i++) for(int j=0;j<M;j++)
        if((i==0||i==N-1||j==0||j==M-1) && !bigRegion[region[i][j]]){
            if(!visited[i][j]){ visited[i][j]=true; q.push({i,j}); }
        }
    while(!q.empty()){
        auto [x,y]=q.front(); q.pop();
        for(int d=0;d<4;d++){
            int nx=x+dx[d],ny=y+dy[d];
            if(nx>=0&&nx<N&&ny>=0&&ny<M&&!visited[nx][ny]&&!bigRegion[region[nx][ny]]){
                visited[nx][ny]=true; q.push({nx,ny});
            }
        }
    }
    
    int ans=0;
    for(int i=0;i<N;i++) for(int j=0;j<M;j++)
        if(bigRegion[region[i][j]] || !visited[i][j]) ans++;
    
    cout << ans << endl;
}
