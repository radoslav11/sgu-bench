#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<pair<int,int>> ARS; // active route states: (route_idx, position)
int main(){
    int n,m,r,S,T;
    cin>>n>>m>>r>>S>>T;
    vector<int> ea(m),eb(m),ec(m);
    for(int i=0;i<m;i++) cin>>ea[i]>>eb[i]>>ec[i];
    vector<vector<int>> routes(r);
    vector<ll> rcost(r,0);
    vector<vector<pair<int,int>>> starts(m); // edge -> list of (route,0)
    for(int i=0;i<r;i++){int k;cin>>k;routes[i].resize(k);for(int j=0;j<k;j++){cin>>routes[i][j];routes[i][j]--;rcost[i]+=ec[routes[i][j]];}starts[routes[i][0]].push_back({i,0});}
    map<pair<int,ARS>,ll> dist;
    map<pair<int,ARS>,pair<ll,pair<int,ARS>>> prev;
    map<pair<int,ARS>,int> pedge;
    priority_queue<pair<ll,pair<int,ARS>>,vector<pair<ll,pair<int,ARS>>>,greater<>> pq;
    ARS init;
    auto ss=make_pair(S,init);
    dist[ss]=0;
    pq.push({0,ss});
    while(!pq.empty()){
        auto [d,st]=pq.top();pq.pop();
        auto [v,ars]=st;
        if(dist.count(st)&&dist[st]<d) continue;
        if(v==T){
            cout<<d<<"\n";
            vector<int> path;
            auto cur=st;
            while(pedge.count(cur)){path.push_back(pedge[cur]+1);cur=prev[cur].second;}
            reverse(path.begin(),path.end());
            cout<<path.size()<<"\n";
            for(int x:path) cout<<x<<" ";cout<<"\n";
            return 0;
        }
        for(int i=0;i<m;i++){
            if(ea[i]!=v) continue;
            int u=eb[i];ll c=ec[i];
            ARS nars;ll pen=0;
            for(auto [ri,pos]:ars){
                if(routes[ri][pos]==i){
                    if(pos+1==(int)routes[ri].size()) pen+=rcost[ri];
                    else nars.push_back({ri,pos+1});
                }
            }
            for(auto [ri,p]:starts[i]){
                bool already=false;
                for(auto [ri2,p2]:ars) if(ri2==ri) {already=true;break;}
                if(!already){
                    if((int)routes[ri].size()==1) pen+=rcost[ri];
                    else nars.push_back({ri,1});
                }
            }
            sort(nars.begin(),nars.end());
            auto ns=make_pair(u,nars);
            ll nd=d+c+pen;
            if(!dist.count(ns)||dist[ns]>nd){
                dist[ns]=nd;
                prev[ns]={d,st};
                pedge[ns]=i;
                pq.push({nd,ns});
            }
        }
    }
    cout<<-1<<"\n";
}
