#include <bits/stdc++.h>
using namespace std;
int main(){
    int a,b,n;
    cin>>a>>b>>n;
    vector<int> c(n);
    for(auto&x:c)cin>>x;
    int pj=0, pg=b;
    vector<pair<int,int>> res;
    bool ok=true;
    for(int i=0;i<n;i++){
        int joined = max(pj, c[i]-pg);
        int girls = c[i]-joined;
        if(joined<0||joined>a||girls<0||girls>pg){ok=false;break;}
        res.push_back({joined-pj, pg-girls});
        pj=joined; pg=girls;
    }
    if(!ok){cout<<"ERROR\n";return 0;}
    for(auto&p:res) cout<<p.first<<" "<<p.second<<"\n";
    return 0;
}
