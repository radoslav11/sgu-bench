#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> pa(n),pb(n);
    for(int i=0;i<n;i++) cin>>pa[i]>>pb[i];
    vector<vector<pair<int,int>>> g(7);
    int deg[7]={};
    for(int i=0;i<n;i++){
        g[pa[i]].push_back({pb[i],i});
        g[pb[i]].push_back({pa[i],i});
        deg[pa[i]]++; deg[pb[i]]++;
    }
    int odd_count=0,start=-1;
    for(int i=0;i<7;i++) if(deg[i]%2==1){odd_count++;start=i;}
    if(odd_count!=0&&odd_count!=2){cout<<"No solution\n";return 0;}
    if(start==-1) for(int i=0;i<7;i++) if(deg[i]>0){start=i;break;}
    if(start==-1){cout<<"No solution\n";return 0;}
    vector<bool> eu(n,false);
    vector<int> ptr(7,0);
    stack<pair<int,int>> st;
    st.push({start,-1});
    vector<pair<int,int>> circuit;
    while(!st.empty()){
        int u=st.top().first;
        bool found=false;
        while(ptr[u]<(int)g[u].size()){
            auto[v,idx]=g[u][ptr[u]++];
            if(!eu[idx]){eu[idx]=true;st.push({v,idx});found=true;break;}
        }
        if(!found){circuit.push_back(st.top());st.pop();}
    }
    if((int)circuit.size()-1!=n){cout<<"No solution\n";return 0;}
    reverse(circuit.begin(),circuit.end());
    for(int i=1;i<(int)circuit.size();i++){
        int from=circuit[i-1].first;
        int eidx=circuit[i].second;
        char sign=(pa[eidx]==from)?'+':'-';
        cout<<(eidx+1)<<" "<<sign<<"\n";
    }
}
