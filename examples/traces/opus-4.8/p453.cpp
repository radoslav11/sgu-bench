#include <bits/stdc++.h>
using namespace std;
int main(){
    long long l,v1,v2,t;
    cin>>l>>v1>>v2>>t;
    long long L2=2*l;
    // case2
    long long count2 = ((v1+v2)*t + l)/L2;
    // case1
    long long count1=0;
    long long d=v1-v2;
    if(d>0){
        long long val=d*t-l;
        if(val>=0) count1 = val/L2 + 1;
        else count1=0;
    } else if(d<0){
        long long dd=-d;
        count1 = (dd*t+l)/L2;
    }
    // overlap
    long long g=__gcd(v1,v2);
    long long M=(g*t)/l;
    long long w=(v1+v2)/g;
    long long overlap=0;
    if(w%2==1) overlap=(M+1)/2;
    cout << (count1+count2-overlap) << "\n";
    return 0;
}
