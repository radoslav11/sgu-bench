#include <bits/stdc++.h>
using namespace std;
bool foldable(vector<int> v){
    sort(v.begin(),v.end());v.erase(unique(v.begin(),v.end()),v.end());
    int l=0,r=(int)v.size()-1;
    while(l<r){int s=v[l]+v[r];while(l<r){if(v[l]+v[r]!=s)return false;l++;r--;}}
    return true;
}
int main(){
    int n,m,k;cin>>n>>m>>k;
    vector<int>X,Y;set<pair<int,int>>S;set<int>sx,sy;
    for(int i=0;i<k;i++){int x,y;cin>>x>>y;X.push_back(x);Y.push_back(y);S.insert({x,y});sx.insert(x);sy.insert(y);}
    bool grid=(long long)sx.size()*sy.size()==(long long)S.size();
    cout<<((grid&&foldable(X)&&foldable(Y))?"YES":"NO")<<endl;
}
