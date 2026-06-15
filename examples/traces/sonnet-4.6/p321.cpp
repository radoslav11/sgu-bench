#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<vector<pair<int,int>>> children(n+1);
    vector<bool> isProtected(n,false);
    for(int i=0;i<n-1;i++){
        int u,v; string p1,p2;
        cin>>u>>v>>p1>>p2;
        children[v].push_back({u,i});
        if(p1=="protected") isProtected[i]=true;
    }
    vector<int> result;
    queue<pair<int,int>> q;
    q.push({1,0});
    while(!q.empty()){
        auto [node,def]=q.front();q.pop();
        for(auto [child,eid]:children[node]){
            int newdef=def+(isProtected[eid]?-1:1);
            if(newdef>0&&!isProtected[eid]){isProtected[eid]=true;result.push_back(eid+1);newdef=def-1;}
            q.push({child,newdef});
        }
    }
    cout<<result.size()<<"\n";
    for(int x:result)cout<<x<<" ";
    cout<<"\n";
}
