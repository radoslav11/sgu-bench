#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<string> v(n);
    for(int i=0;i<n;i++) cin>>v[i];
    sort(v.begin(),v.end(),[](const string&a,const string&b){
        return a+b < b+a;
    });
    for(auto&s:v) cout<<s;
    cout<<endl;
}
