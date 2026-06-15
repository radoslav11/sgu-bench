#include<bits/stdc++.h>
using namespace std;
int main(){
    long long H,W,Rc,Cc,Rt,Ct;
    char who;
    cin>>H>>W>>Rc>>Cc>>Rt>>Ct>>who;
    long long d=abs(Rc-Rt)+abs(Cc-Ct);
    if(H>=2&&W>=2){cout<<0;return 0;}
    if(who=='C'){cout<<2*d-1;}
    else{cout<<2*d;}
}
