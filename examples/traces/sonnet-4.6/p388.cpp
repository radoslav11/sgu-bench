#include <bits/stdc++.h>
using namespace std;

// Bipartite matching
int match_l[105], match_r[105];
bool used[105];
vector<int> adj[105];
int NL, NR;

bool dfs(int v) {
    for (int to : adj[v]) {
        if (!used[to]) {
            used[to] = true;
            if (match_r[to] == -1 || dfs(match_r[to])) {
                match_l[v] = to;
                match_r[to] = v;
                return true;
            }
        }
    }
    return false;
}

int main(){
    int n,m1,m2;
    cin>>n>>m1>>m2;
    int half=n/2;
    vector<pair<int,int>> juan(m1), rosa(m2);
    for(auto&p:juan) {cin>>p.first>>p.second; p.first--; p.second--;}
    for(auto&p:rosa) {cin>>p.first>>p.second; p.first--; p.second--;}
    
    vector<bool> men(half,true), women(half,true);
    // iterative trimming
    for(int iter=0;iter<200;iter++){
        // Juan matching
        for(int i=0;i<half;i++) adj[i].clear();
        for(auto&p:juan) if(men[p.first]&&women[p.second-half]) adj[p.first].push_back(p.second-half);
        memset(match_l,-1,sizeof(match_l)); memset(match_r,-1,sizeof(match_r));
        for(int i=0;i<half;i++) if(men[i]){memset(used,0,sizeof(used));dfs(i);}
        for(int i=0;i<half;i++) if(men[i]&&match_l[i]==-1) men[i]=false;
        for(int j=0;j<half;j++) if(women[j]&&match_r[j]==-1) women[j]=false;
        // Rosa matching
        for(int i=0;i<half;i++) adj[i].clear();
        for(auto&p:rosa) if(men[p.first]&&women[p.second-half]) adj[p.first].push_back(p.second-half);
        memset(match_l,-1,sizeof(match_l)); memset(match_r,-1,sizeof(match_r));
        for(int i=0;i<half;i++) if(men[i]){memset(used,0,sizeof(used));dfs(i);}
        for(int i=0;i<half;i++) if(men[i]&&match_l[i]==-1) men[i]=false;
        for(int j=0;j<half;j++) if(women[j]&&match_r[j]==-1) women[j]=false;
    }
    int k=0; for(int i=0;i<half;i++) if(men[i]) k++;
    cout<<k<<"\n";
    // Output Juan matching
    for(int i=0;i<half;i++) adj[i].clear();
    for(auto&p:juan) if(men[p.first]&&women[p.second-half]) adj[p.first].push_back(p.second-half);
    memset(match_l,-1,sizeof(match_l)); memset(match_r,-1,sizeof(match_r));
    for(int i=0;i<half;i++) if(men[i]){memset(used,0,sizeof(used));dfs(i);}
    for(int i=0;i<half;i++) if(men[i]) cout<<i+1<<" "<<match_l[i]+half+1<<"\n";
    // Output Rosa matching
    for(int i=0;i<half;i++) adj[i].clear();
    for(auto&p:rosa) if(men[p.first]&&women[p.second-half]) adj[p.first].push_back(p.second-half);
    memset(match_l,-1,sizeof(match_l)); memset(match_r,-1,sizeof(match_r));
    for(int i=0;i<half;i++) if(men[i]){memset(used,0,sizeof(used));dfs(i);}
    for(int i=0;i<half;i++) if(men[i]) cout<<i+1<<" "<<match_l[i]+half+1<<"\n";
}
