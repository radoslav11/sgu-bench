#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<int> a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    vector<int> idx(n);
    iota(idx.begin(),idx.end(),1);
    sort(idx.begin(),idx.end(),[&](int x,int y){return a[x]<a[y];});
    for(int i=0;i<n;i++) if(a[idx[i]]!=i){cout<<"NO\n";return 0;}
    cout<<"YES\n";
    for(int i=0;i<n;i++) cout<<-idx[i]<<" "<<idx[i]<<" ";
    cout<<"\n";
}
