#include <bits/stdc++.h>
using namespace std;
int N,M,K;
vector<string> g;
int main(){
    scanf("%d %d %d",&N,&M,&K);
    g.resize(N);
    for(int i=0;i<N;i++){char buf[600];scanf("%s",buf);g[i]=buf;}
    vector<int> comp(N*M,-1);
    vector<int> sz;
    vector<vector<int>> cells;
    int dx[]={0,0,1,-1},dy[]={1,-1,0,0};
    int nc=0;
    vector<int> stk;
    for(int i=0;i<N;i++)for(int j=0;j<M;j++){
        if(comp[i*M+j]==-1){
            stk.clear();stk.push_back(i*M+j);comp[i*M+j]=nc;
            vector<int> cur;
            char c=g[i][j];
            while(!stk.empty()){
                int p=stk.back();stk.pop_back();cur.push_back(p);
                int x=p/M,y=p%M;
                for(int d=0;d<4;d++){int nx=x+dx[d],ny=y+dy[d];if(nx<0||ny<0||nx>=N||ny>=M)continue;if(comp[nx*M+ny]==-1&&g[nx][ny]==c){comp[nx*M+ny]=nc;stk.push_back(nx*M+ny);}}
            }
            sz.push_back(cur.size());
            cells.push_back(cur);
            nc++;
        }
    }
    vector<char> dangerous(N*M,0);
    // mark big region cells
    for(int r=0;r<nc;r++) if(sz[r]>=K) for(int p:cells[r]) dangerous[p]=1;
    // for each big region, flood from border avoiding it
    vector<char> vis(N*M,0);
    for(int r=0;r<nc;r++){
        if(sz[r]<K) continue;
        // BFS from border cells not in region r
        // reset vis only on touched - use global counter approach
        // simple: fill vis 0 each time but that's NM per region; total cost ok-ish
        fill(vis.begin(),vis.end(),0);
        // mark region cells as vis (blocked)
        for(int p:cells[r]) vis[p]=1;
        deque<int> q;
        for(int j=0;j<M;j++){
            for(int i:{0,N-1}){
                int p=i*M+j;
                if(!vis[p]){vis[p]=1;q.push_back(p);}
            }
        }
        for(int i=0;i<N;i++){
            for(int j:{0,M-1}){
                int p=i*M+j;
                if(!vis[p]){vis[p]=1;q.push_back(p);}
            }
        }
        while(!q.empty()){
            int p=q.front();q.pop_front();
            int x=p/M,y=p%M;
            for(int d=0;d<4;d++){int nx=x+dx[d],ny=y+dy[d];if(nx<0||ny<0||nx>=N||ny>=M)continue;int np=nx*M+ny;if(!vis[np]){vis[np]=1;q.push_back(np);}}
        }
        // cells not visited and not in region r are trapped by r
        for(int p=0;p<N*M;p++){
            if(!vis[p]) dangerous[p]=1;
        }
    }
    long long ans=0;
    for(int p=0;p<N*M;p++) if(dangerous[p]) ans++;
    printf("%lld\n",ans);
}
