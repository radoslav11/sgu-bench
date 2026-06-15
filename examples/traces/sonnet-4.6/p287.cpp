#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;
int main(){
    ll q,c;
    cin>>q>>c;
    // f(n) = max q handleable with n questions, delay c
    // f(n) = 1 for 1<=n<=c, f(0)=0
    // f(n) = f(n-1) + f(n-c-1) + 1 for n>c (but c+1 questions "wasted" at start? let's try c+1)
    // Try: first question answered after c more questions, so branch at cost c+1
    // f(n) = f(n-1) + f(n-1-c) + 1, n>=1+c; f(k)=1 for 1<=k<=c; f(0)=0
    ll cap=2e15;
    vector<lll> f(2000000+10,0);
    f[0]=0;
    for(int i=1;i<=(int)c && i<(int)f.size();i++) f[i]=1;
    for(int i=c+1;i<(int)f.size();i++){
        f[i]=f[i-1]+(i-1-c>=0?f[i-1-c]:0)+1;
        if(f[i]>(lll)cap) f[i]=(lll)cap+1;
        if(f[i]>=(lll)q){cout<<i;return 0;}
    }
    cout<<(int)f.size()-1;
}
