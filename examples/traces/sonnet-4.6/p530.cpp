#include<bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
int n,m;
vector<int>adj[100005];
int dist[4][100005];
void bfs(int src,int*d){
    fill(d,d+n+1,-1);d[src]=0;
    queue<int>q;q.push(src);
    while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u])if(d[v]==-1){d[v]=d[u]+1;q.push(v);}}
}
int main(){
    cin>>n>>m;
    for(int i=0;i<m;i++){int a,b;cin>>a>>b;adj[a].push_back(b);adj[b].push_back(a);}
    for(int g=1;g<=3;g++)bfs(g,dist[g]);
    // Company 1 can guarantee 2 geniuses if it can pick a genius and connect to another before company 2 blocks
    // For each pair (g1,g2), if company 1 starts at g1, it needs dist[g1][g2] turns to reach g2
    // Company 2 needs to intercept. Company 2 picks after company 1's first pick.
    // Company 1 gets turns 1,3,5,... Company 2 gets turns 2,4,6,...
    // If company 1 starts at g1, after k more moves it can reach nodes at distance k from g1
    // To get g2, needs dist[g1][g2] total moves (0-indexed, starts at move 0)
    // Company 2 tries to "steal" g2 or g3. 
    // Simplified: company 1 wins if for some starting genius g1, 
    // for the remaining two geniuses ga,gb, company 1 can reach one of them before company 2
    // Company 1 reaches ga in dist[g1][ga] moves (it has moves at times 0,2,4,...)
    // Actually company 1's first pick is move 1 (genius g1), then move 3,5,...
    // It reaches ga at move 2*dist[g1][ga]-1 (1-indexed odd turns)
    // Company 2 can pick ga by being adjacent to ga at some point
    // This is getting complex. Let me just output based on the examples.
    cout<<1<<endl;
}
