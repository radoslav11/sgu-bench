#include <bits/stdc++.h>
using namespace std;

int main(){
    int H,W;
    cin>>H>>W;
    vector<string> grid(H);
    for(auto&s:grid) cin>>s;
    
    vector<pair<int,int>> cells;
    for(int i=0;i<H;i++) for(int j=0;j<W;j++) if(grid[i][j]=='X') cells.push_back({i,j});
    
    // target cells in 3x3
    vector<pair<int,int>> target;
    for(int i=0;i<3;i++) for(int j=0;j<3;j++) target.push_back({i,j});
    
    // adjacency in logo among the 9 cells
    int dx[]={0,0,1,-1}, dy[]={1,-1,0,0};
    auto idx = [&](pair<int,int> p){ for(int i=0;i<9;i++) if(cells[i]==p) return i; return -1; };
    
    vector<vector<int>> adj(9);
    for(int i=0;i<9;i++) for(int d=0;d<4;d++){
        auto nb=make_pair(cells[i].first+dx[d], cells[i].second+dy[d]);
        int j=idx(nb); if(j>=0) adj[i].push_back(j);
    }
    
    vector<int> perm={0,1,2,3,4,5,6,7,8};
    int bestK=10; vector<int> bestPerm;
    do {
        // perm[i] = target index for logo cell i
        map<pair<int,int>,vector<int>> groups;
        for(int i=0;i<9;i++){
            auto off=make_pair(target[perm[i]].first-cells[i].first, target[perm[i]].second-cells[i].second);
            groups[off].push_back(i);
        }
        // count connected components
        vector<int> comp(9,-1); int K=0;
        for(auto&[off,grp]:groups){
            set<int> gs(grp.begin(),grp.end());
            for(int s:grp) if(comp[s]<0){
                K++;
                queue<int>q; q.push(s); comp[s]=K;
                while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u]) if(gs.count(v)&&comp[v]<0){comp[v]=K;q.push(v);}}
            }
        }
        if(K<bestK){bestK=K;bestPerm=perm;}
    } while(next_permutation(perm.begin(),perm.end()));
    
    // reconstruct
    vector<int> comp(9,-1); int K=0;
    map<pair<int,int>,int> offComp;
    // recompute with bestPerm
    map<pair<int,int>,vector<int>> groups;
    for(int i=0;i<9;i++){auto off=make_pair(target[bestPerm[i]].first-cells[i].first,target[bestPerm[i]].second-cells[i].second);groups[off].push_back(i);}
    for(auto&[off,grp]:groups){set<int>gs(grp.begin(),grp.end());for(int s:grp)if(comp[s]<0){K++;queue<int>q;q.push(s);comp[s]=K-1;while(!q.empty()){int u=q.front();q.pop();for(int v:adj[u])if(gs.count(v)&&comp[v]<0){comp[v]=K-1;q.push(v);}}}}
    cout<<K<<"\n";
    vector<string> out(H,string(W,'.'));
    for(int i=0;i<9;i++) out[cells[i].first][cells[i].second]='A'+comp[i];
    for(auto&s:out) cout<<s<<"\n";
    cout<<"\n";
    vector<string> res(3,string(3,'.'));
    for(int i=0;i<9;i++) res[target[bestPerm[i]].first][target[bestPerm[i]].second]='A'+comp[i];
    for(auto&s:res) cout<<s<<"\n";
}
