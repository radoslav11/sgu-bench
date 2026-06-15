#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<string> v(n);
    for(auto&s:v)cin>>s;
    sort(v.begin(),v.end(),[](const string&a,const string&b){return a+b<b+a;});
    for(auto&s:v)cout<<s;
    cout<<"\n";
}
