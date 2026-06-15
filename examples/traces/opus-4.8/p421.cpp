#include <bits/stdc++.h>
using namespace std;
int n,m;long long k;
vector<long long> a;
vector<long long> prods;
void rec(int idx,int cnt,long long cur){
    if(cnt==m){prods.push_back(cur);return;}
    if(idx>=n)return;
    if(n-idx<m-cnt)return;
    rec(idx+1,cnt+1,cur*a[idx]);
    rec(idx+1,cnt,cur);
}
int main(){
    cin>>n>>m>>k;a.resize(n);for(auto&x:a)cin>>x;
    rec(0,0,1);
    sort(prods.rbegin(),prods.rend());
    cout<<prods[k-1]<<endl;
}
