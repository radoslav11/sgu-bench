#include<bits/stdc++.h>
using namespace std;
int main(){
    long long n;
    cin>>n;
    long long q=n/3, r=n%3;
    long long ans=2*q+(r>=2?1:0);
    cout<<ans<<endl;
    return 0;
}
