#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    vector<string> grid(N);
    for(int i = 0; i < N; i++) cin >> grid[i];
    
    vector<vector<int>> cnt(N, vector<int>(M, 0));
    vector<vector<bool>> weed(N, vector<bool>(M, false));
    
    int dx[] = {0,0,1,-1};
    int dy[] = {1,-1,0,0};
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(grid[i][j] == 'X') weed[i][j] = true;
    
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(!weed[i][j])
                for(int d = 0; d < 4; d++){
                    int ni = i+dx[d], nj = j+dy[d];
                    if(ni>=0 && ni<N && nj>=0 && nj<M && weed[ni][nj])
                        cnt[i][j]++;
                }
    
    queue<pair<int,int>> q;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(!weed[i][j] && cnt[i][j] >= 2)
                q.push({i,j});
    
    while(!q.empty()){
        auto [x,y] = q.front(); q.pop();
        if(weed[x][y]) continue;
        weed[x][y] = true;
        for(int d = 0; d < 4; d++){
            int nx = x+dx[d], ny = y+dy[d];
            if(nx>=0 && nx<N && ny>=0 && ny<M && !weed[nx][ny]){
                cnt[nx][ny]++;
                if(cnt[nx][ny] >= 2)
                    q.push({nx,ny});
            }
        }
    }
    
    int ans = 0;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(weed[i][j]) ans++;
    
    cout << ans << endl;
    
    return 0;
}
