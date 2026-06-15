#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;cin>>n;
    vector<unsigned long long> a(n);
    for(auto&x:a)cin>>x;
    vector<unsigned long long> basis;
    for(auto x:a){
        for(auto b:basis) x=min(x,x^b);
        if(x){basis.push_back(x);sort(basis.rbegin(),basis.rend());}
    }
    unsigned long long res=0;
    for(auto b:basis) res=max(res,res^b);
    cout<<res<<endl;
}
