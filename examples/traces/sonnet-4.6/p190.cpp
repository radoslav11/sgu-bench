#include <bits/stdc++.h>
using namespace std;

int N, P;
bool removed[41][41];
int id[41][41]; // id for black squares
int match_b[1601], match_w[1601]; // matching
vector<int> adj[1601]; // black -> list of white ids
int wid[41][41];
bool visited[1601];

int bcount = 0, wcount = 0;

bool dfs(int u) {
    for (int v : adj[u]) {
        if (!visited[v]) {
            visited[v] = true;
            if (match_w[v] == -1 || dfs(match_w[v])) {
                match_b[u] = v;
                match_w[v] = u;
                return true;
            }
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> N >> P;
    memset(removed, false, sizeof(removed));
    for (int i = 0; i < P; i++) {
        int x, y; cin >> x >> y;
        removed[x][y] = true;
    }
    
    memset(id, -1, sizeof(id));
    memset(wid, -1, sizeof(wid));
    
    for (int x = 1; x <= N; x++)
        for (int y = 1; y <= N; y++)
            if (!removed[x][y]) {
                if ((x+y) % 2 == 0) id[x][y] = bcount++;
                else wid[x][y] = wcount++;
            }
    
    int dx[] = {1,-1,0,0};
    int dy[] = {0,0,1,-1};
    
    for (int x = 1; x <= N; x++)
        for (int y = 1; y <= N; y++)
            if (!removed[x][y] && (x+y)%2==0) {
                int u = id[x][y];
                for (int d = 0; d < 4; d++) {
                    int nx = x+dx[d], ny = y+dy[d];
                    if (nx>=1&&nx<=N&&ny>=1&&ny<=N&&!removed[nx][ny]) {
                        adj[u].push_back(wid[nx][ny]);
                    }
                }
            }
    
    if (bcount != wcount) { cout << "No\n"; return 0; }
    
    memset(match_b, -1, sizeof(match_b));
    memset(match_w, -1, sizeof(match_w));
    
    int matched = 0;
    for (int u = 0; u < bcount; u++) {
        memset(visited, false, sizeof(visited));
        if (dfs(u)) matched++;
    }
    
    if (matched != bcount) { cout << "No\n"; return 0; }
    
    vector<pair<int,int>> horiz, vert;
    // find which white square corresponds to each black square
    int rw[1601][2], rb[1601][2];
    for (int x=1;x<=N;x++) for(int y=1;y<=N;y++) {
        if (!removed[x][y]&&(x+y)%2==0) { rb[id[x][y]][0]=x; rb[id[x][y]][1]=y; }
        if (!removed[x][y]&&(x+y)%2==1) { rw[wid[x][y]][0]=x; rw[wid[x][y]][1]=y; }
    }
    for (int u=0;u<bcount;u++) {
        int v=match_b[u];
        int bx=rb[u][0],by=rb[u][1],wx=rw[v][0],wy=rw[v][1];
        if (by==wy) { int lx=min(bx,wx); horiz.push_back({lx,by}); }
        else { int bot=min(by,wy); vert.push_back({bx,bot}); }
    }
    cout<<"Yes\n";
    cout<<horiz.size()<<"\n";
    for (auto&p:horiz) cout<<p.first<<" "<<p.second<<"\n";
    cout<<vert.size()<<"\n";
    for (auto&p:vert) cout<<p.first<<" "<<p.second<<"\n";
}
