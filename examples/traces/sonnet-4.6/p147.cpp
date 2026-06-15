#include<bits/stdc++.h>
using namespace std;
int main(){
    long long n,p1,q1,p2,q2,p3,q3;
    cin>>n>>p1>>q1>>p2>>q2>>p3>>q3;
    long long D=max(abs(p1-p2),abs(q1-q2));
    long long dw=max(abs(p3-p2),abs(q3-q2));
    long long db=max(abs(p3-p1),abs(q3-q1));
    long long meet=(D+1)/2;
    long long bw=min(dw,db);
    if(bw<meet){
        cout<<"YES"<<endl<<bw<<endl;
    } else {
        cout<<"NO"<<endl<<D<<endl;
    }
}
