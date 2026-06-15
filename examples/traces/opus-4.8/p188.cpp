#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; long long T;
    cin>>n>>T;
    vector<long long> L(n), V(n);
    for(auto&x:L)cin>>x;
    for(auto&x:V)cin>>x;
    vector<long long> B(n,0);
    for(int i=0;i<n;i++)for(int j=i+1;j<n;j++){
        if((V[i]>0) == (V[j]>0)) continue;
        long long D=V[i]-V[j];
        long long cnt=0;
        for(long long k=-200000;k<=200000;k++){
            long long num=(L[j]-L[i]+1000*k);
            __int128 nn=num, dd=D;
            if(dd<0){nn=-nn; dd=-dd;}
            if(nn<0) continue;
            if(nn > (__int128)T*dd) continue;
            cnt++;
        }
        B[i]+=cnt; B[j]+=cnt;
    }
    for(int i=0;i<n;i++){cout<<B[i];if(i+1<n)cout<<" ";}
    cout<<"\n";
    return 0;
}
