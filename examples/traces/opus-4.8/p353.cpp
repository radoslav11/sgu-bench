#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N,k1,k2,p1,p2,p3;
    cin>>N>>k1>>k2>>p1>>p2>>p3;
    if(N<p1){cout<<0;return 0;}
    long long sec=k1;
    long long b=N-p1;
    if(b>0){
        long long can = (b + p2 -1)/p2;
        long long take = min(can,k2);
        sec+=take;
        b-=take*p2;
    }
    if(b>0){
        long long can=(b+p3-1)/p3;
        sec+=can;
        b-=can*p3;
    }
    cout<<sec<<endl;
}
