#include <bits/stdc++.h>
using namespace std;

int N, P;
bool removed[41][41];
int id[41][41]; // cell id for present cells
int matchTo[1700];
vector<int> adj[1700];
bool used[1700];

int cellIndex(int x, int y){ return id[x][y]; }

bool tryK(int u){
    for(int v: adj[u]){
        if(!used[v]){
            used[v]=true;
            if(matchTo[v]==-1 || tryK(matchTo[v])){
                matchTo[v]=u;
                return true;
            }
        }
    }
    return false;
}

int main(){
    scanf("%d %d",&N,&P);
    for(int i=0;i<P;i++){
        int x,y; scanf("%d %d",&x,&y);
        removed[x][y]=true;
    }
    // assign ids
    int cnt=0;
    int blackCount=0, whiteCount=0;
    for(int x=1;x<=N;x++)for(int y=1;y<=N;y++){
        id[x][y]=-1;
        if(!removed[x][y]){
            id[x][y]=cnt++;
        }
    }
    // present cells count
    int present = cnt;
    if(present % 2 !=0){
        printf("No\n");
        return 0;
    }
    // build bipartite: color = (x+y)%2, black=0 left side
    // we match black cells to white cells
    int dx[4]={1,-1,0,0};
    int dy[4]={0,0,1,-1};
    for(int x=1;x<=N;x++)for(int y=1;y<=N;y++){
        if(removed[x][y]) continue;
        if(((x+y)&1)==0){ // black, left side
            int u=id[x][y];
            for(int d=0;d<4;d++){
                int nx=x+dx[d], ny=y+dy[d];
                if(nx<1||nx>N||ny<1||ny>N) continue;
                if(removed[nx][ny]) continue;
                adj[u].push_back(id[nx][ny]);
            }
        }
    }
    for(int i=0;i<cnt;i++) matchTo[i]=-1;
    int matchCount=0;
    // iterate over black cells
    for(int x=1;x<=N;x++)for(int y=1;y<=N;y++){
        if(removed[x][y]) continue;
        if(((x+y)&1)==0){
            int u=id[x][y];
            for(int i=0;i<cnt;i++) used[i]=false;
            if(tryK(u)) matchCount++;
        }
    }
    if(matchCount*2 != present){
        printf("No\n");
        return 0;
    }
    // reconstruct dominoes
    // for each white cell v matched to black u, find coords
    // store coords by id
    int cx[1700], cy[1700];
    for(int x=1;x<=N;x++)for(int y=1;y<=N;y++){
        if(!removed[x][y]){ cx[id[x][y]]=x; cy[id[x][y]]=y; }
    }
    vector<pair<int,int>> horiz, vert;
    for(int v=0;v<cnt;v++){
        if(matchTo[v]!=-1){
            int u=matchTo[v];
            int ux=cx[u], uy=cy[u];
            int vx=cx[v], vy=cy[v];
            if(uy==vy){
                // horizontal, left square = min x
                horiz.push_back({min(ux,vx), uy});
            } else {
                // vertical, bottom square = min y
                vert.push_back({ux, min(uy,vy)});
            }
        }
    }
    printf("Yes\n");
    printf("%d\n",(int)horiz.size());
    for(auto&p:horiz) printf("%d %d\n",p.first,p.second);
    printf("%d\n",(int)vert.size());
    for(auto&p:vert) printf("%d %d\n",p.first,p.second);
    return 0;
}
