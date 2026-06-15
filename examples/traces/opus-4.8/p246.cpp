#include <bits/stdc++.h>
using namespace std;
int main(){
    long long M; cin>>M;
    long long N=(M+1)/2;
    long long d=__gcd(N-2,3LL);
    long long ans;
    if(d==1) ans=N;
    else { long long L=M/3; ans=3*((L-1)/2)+1; }
    cout<<ans<<endl;
}
