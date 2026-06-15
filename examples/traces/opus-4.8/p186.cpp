#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> L(n);
    for(auto&x:L)cin>>x;
    sort(L.begin(),L.end());
    int best=INT_MAX;
    int prefix=0;
    for(int d=0;d<=n;d++){
        int remaining=n-d;
        int need=remaining-1;
        if(need<0)need=0;
        if(prefix>=need){
            best=min(best,prefix);
        } else {
            int extra=need-prefix;
            best=min(best,prefix+extra);
        }
        if(d<n)prefix+=L[d];
    }
    cout<<best<<endl;
}
