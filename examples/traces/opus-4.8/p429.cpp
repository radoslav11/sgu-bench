#include<bits/stdc++.h>
using namespace std;
map<vector<int>,int> memo;
bool win(vector<int> v){
    while(!v.empty()&&v.front()==0)v.erase(v.begin());
    while(!v.empty()&&v.back()==0)v.pop_back();
    if(v.empty())return false;
    auto it=memo.find(v);
    if(it!=memo.end())return it->second;
    bool res=false;
    int n=v.size();
    for(int t=1;t<=v.front()&&!res;t++){auto w=v;w[0]-=t;if(!win(w))res=true;}
    for(int t=1;t<=v.back()&&!res;t++){auto w=v;w[n-1]-=t;if(!win(w))res=true;}
    memo[v]=res;
    return res;
}
int main(){int n;cin>>n;vector<int>v(n);for(auto&x:v)cin>>x;cout<<(win(v)?"FEDOR":"SERGEY")<<endl;}
