#include <bits/stdc++.h>
using namespace std;
int main(){
    int a, b, n;
    cin >> a >> b >> n;
    vector<int> t(n);
    for(int i=0;i<n;i++) cin >> t[i];
    int boys_in=0, girls_in=b, boys_avail=a;
    vector<pair<int,int>> ans;
    bool ok=true;
    for(int i=0;i<n;i++){
        int cur=boys_in+girls_in;
        int delta=t[i]-cur;
        int j=0,g=0;
        if(delta>=0){j=delta;g=0;if(j>boys_avail){ok=false;break;}}
        else{g=-delta;j=0;if(g>girls_in){j=girls_in+delta;g=girls_in;if(j<0||j>boys_avail){ok=false;break;}}}
        ans.push_back({j,g});
        boys_in+=j;girls_in-=g;boys_avail-=j;
    }
    if(!ok)cout<<"ERROR\n";
    else for(auto&p:ans)cout<<p.first<<" "<<p.second<<"\n";
}
