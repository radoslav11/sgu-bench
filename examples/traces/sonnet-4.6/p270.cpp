#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M;
    cin>>N>>M;
    vector<pair<int,int>> ops(M);
    for(auto &p:ops) {cin>>p.first>>p.second; p.first--;p.second--;}
    // BFS: state=(ball_pos, sorted vector of remaining op indices as multiset counts)
    // Use map from distinct ops to counts
    map<pair<int,int>,int> opcount;
    for(auto &p:ops) opcount[p]++;
    vector<pair<int,int>> distops;
    vector<int> initcounts;
    for(auto &kv:opcount){distops.push_back(kv.first);initcounts.push_back(kv.second);}
    int D=distops.size();
    typedef pair<int,vector<int>> State;
    set<State> visited;
    queue<State> q;
    State init={0,initcounts};
    q.push(init);visited.insert(init);
    set<int> reachable;
    while(!q.empty()){
        auto [pos,cnt]=q.front();q.pop();
        bool any=false;
        for(int i=0;i<D;i++) if(cnt[i]>0){any=true;
            auto [a,b]=distops[i];
            cnt[i]--;
            int npos=(pos==a)?b:(pos==b)?a:pos;
            State ns={npos,cnt};
            if(!visited.count(ns)){visited.insert(ns);q.push(ns);}
            cnt[i]++;
        }
        if(!any) reachable.insert(pos);
    }
    for(int x:reachable) cout<<x+1<<" ";
}
