#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<int> x(n),y(n);
    map<int,vector<int>> byX,byY;
    for(int i=0;i<n;i++){
        cin>>x[i]>>y[i];
        byX[x[i]].push_back(y[i]);
        byY[y[i]].push_back(x[i]);
    }
    if(n%2!=0){cout<<0;return 0;}
    long long ans=0;
    for(auto&p:byX){
        auto&v=p.second;
        if(v.size()%2!=0){cout<<0;return 0;}
        sort(v.begin(),v.end());
        for(int i=0;i<(int)v.size();i+=2)
            ans+=v[i+1]-v[i];
    }
    for(auto&p:byY){
        auto&v=p.second;
        if(v.size()%2!=0){cout<<0;return 0;}
        sort(v.begin(),v.end());
        for(int i=0;i<(int)v.size();i+=2)
            ans+=v[i+1]-v[i];
    }
    cout<<ans;
}
