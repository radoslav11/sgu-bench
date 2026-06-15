#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,k;
    cin>>n>>k;
    if(k>n){cout<<0<<endl;return 0;}
    // C(n,k)^2 * k!
    long long C=1;
    for(int i=0;i<k;i++) C=C*(n-i)/(i+1);
    long long ans=C*C;
    for(int i=1;i<=k;i++) ans*=i;
    cout<<ans<<endl;
}
