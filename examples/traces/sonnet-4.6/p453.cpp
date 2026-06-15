#include <bits/stdc++.h>
using namespace std;
int main(){
    long long l,v1,v2,t;
    cin>>l>>v1>>v2>>t;
    // type A: (v1+v2)*t = (2k+1)*l, k>=0
    // count of k: (2k+1)*l <= (v1+v2)*t => k <= ((v1+v2)*t/l - 1)/2
    __int128 S=(__int128)(v1+v2)*t;
    __int128 L=l;
    long long cntA=0;
    if(S>=L) cntA=(long long)((S/L+1)/2); // number of odd multiples of l <= S
    // type B: (v1-v2)*t = (2k-1)*l, k>=1, only if v1!=v2
    long long cntB=0;
    if(v1!=v2){
        long long dv=abs(v1-v2);
        __int128 S2=(__int128)dv*t;
        if(S2>=L) cntB=(long long)((S2/L+1)/2);
    }
    cout<<cntA+cntB<<endl;
}
