#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> t(n),l(n);
    for(auto&x:t)cin>>x;
    for(auto&x:l)cin>>x;
    vector<int> idx(n);
    for(int i=0;i<n;i++)idx[i]=i;
    sort(idx.begin(),idx.end(),[&](int a,int b){return l[a]>l[b];});
    int cur=0,ans=0;
    for(int i:idx){
        cur+=t[i];
        ans=max(ans,cur+l[i]);
    }
    cout<<ans<<endl;
}
