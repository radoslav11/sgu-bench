#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin>>n>>m;
    vector<string> v(m);
    for(auto&s:v)cin>>s;
    cout<<v[(n-1)%m]<<endl;
}
