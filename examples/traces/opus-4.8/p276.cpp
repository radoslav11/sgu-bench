#include <bits/stdc++.h>
using namespace std;
int main(){
    long long s,p;
    cin>>s>>p;
    long long d=p-s;
    int c;
    if(d<=0)c=0;
    else if(d<300)c=1;
    else if(d<900)c=2;
    else if(d<1800)c=3;
    else c=4;
    cout<<c<<endl;
}
