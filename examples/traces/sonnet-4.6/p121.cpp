#include <bits/stdc++.h>
using namespace std;
int N,M=0;
vector<pair<int,int>> adj[101];
int col[5000];
pair<int,int> edges[5000];
bool bt(int i){
    if(i==M){
        for(int u=1;u<=N;u++){
            if((int)adj[u].size()<2)continue;
            bool h1=false,h2=false;
            for(auto&p:adj[u]){if(col[p.second]==1)h1=true;else h2=true;}
            if(!h1||!h2)return false;
        }
        return true;
    }
    for(int c=1;c<=2;c++){
        col[i]=c;
        if(bt(i+1))return true;
    }
    return false;
}
int main(){
    cin>>N;
    set<pair<int,int>>seen;
    vector<vector<int>> raw(N+1);
    for(int u=1;u<=N;u++){
        int v;
        while(cin>>v&&v){
            raw[u].push_back(v);
            if(!seen.count({min(u,v),max(u,v)})){
                seen.insert({min(u,v),max(u,v)});
                edges[M]={u,v};
                adj[u].push_back({v,M});
                adj[v].push_back({u,M});
                M++;
            }
        }
    }
    if(!bt(0)){cout<<"No solution"<<endl;}
    else{
        for(int u=1;u<=N;u++){
            for(int v:raw[u]){
                // find edge id between u and v
                int eid=-1;
                for(auto&p:adj[u])if(p.first==v){eid=p.second;break;}
                cout<<col[eid]<<" ";
            }
            cout<<"0\n";
        }
    }
}
