#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int main(){
    int n; ll p;
    cin>>n>>p;
    vector<vector<ll>> v(n,vector<ll>(n));
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) cin>>v[i][j];
    ll ans=1;
    for(int i=0;i<n;i++){
        ll g=0;
        for(int j=0;j<n;j++) g=__gcd(g,v[i][j]);
        ans=ans%p*(2*g%p)%p;
    }
    cout<<ans%p<<endl;
}
