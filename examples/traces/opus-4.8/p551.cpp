#include <bits/stdc++.h>
using namespace std;
int main(){
    long long n,t1,t2;
    cin>>n>>t1>>t2;
    long long last1=0,last2=0;
    bool active1=true,active2=true;
    long long totalFinished=0,lastTime=0;
    while(active1||active2){
        long long n1 = active1? last1+t1 : LLONG_MAX;
        long long n2 = active2? last2+t2 : LLONG_MAX;
        long long t=min(n1,n2);
        if(active1 && n1==t){last1=t;totalFinished++;}
        if(active2 && n2==t){last2=t;totalFinished++;}
        lastTime=t;
        if(active1 && last1==t) active1 = (totalFinished < n);
        if(active2 && last2==t) active2 = (totalFinished < n);
    }
    cout<<totalFinished<<" "<<lastTime<<"\n";
}
