#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
    vector<set<int>> res(n+1);
    for(int u=1;u<=m;u++){
        int k;cin>>k;
        for(int j=0;j<k;j++){int r;cin>>r;res[r].insert(u);}
    }
    set<set<int>> groups;
    for(int r=1;r<=n;r++) if(!res[r].empty()) groups.insert(res[r]);
    cout<<groups.size()<<endl;
}
