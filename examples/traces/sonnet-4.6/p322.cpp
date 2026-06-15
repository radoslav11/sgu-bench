#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    set<pair<int,int>> e1,e2;
    vector<set<int>> adj1(n+1);
    for(int i=0;i<n-1;i++){int a,b;cin>>a>>b;e1.insert({min(a,b),max(a,b)});adj1[a].insert(b);adj1[b].insert(a);}
    for(int i=0;i<n-1;i++){int a,b;cin>>a>>b;e2.insert({min(a,b),max(a,b)});}
    vector<tuple<int,int,int,int,int>> ops;
    while(e1!=e2){
        pair<int,int> target={-1,-1};
        for(auto&p:e2) if(!e1.count(p)){target=p;break;}
        int a=target.first,b=target.second;
        vector<int> par(n+1,-1);
        vector<bool> vis(n+1,false);
        queue<int> q;
        q.push(a);vis[a]=true;
        while(!q.empty()){int u=q.front();q.pop();for(int v:adj1[u])if(!vis[v]){vis[v]=true;par[v]=u;q.push(v);}}
        int cur=b;
        pair<int,int> rem={-1,-1};
        while(cur!=a){
            int p=par[cur];
            pair<int,int> ed={min(p,cur),max(p,cur)};
            if(!e2.count(ed)){rem=ed;break;}
            cur=p;
        }
        ops.push_back({1,a,b,rem.first,rem.second});
        e1.insert(target);adj1[a].insert(b);adj1[b].insert(a);
        e1.erase(rem);adj1[rem.first].erase(rem.second);adj1[rem.second].erase(rem.first);
    }
    cout<<ops.size()<<"\n";
    for(auto&[p,a1,b1,a2,b2]:ops) cout<<p<<" "<<a1<<" "<<b1<<" "<<a2<<" "<<b2<<"\n";
}
