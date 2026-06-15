#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n; long long L;
    cin>>n>>L;
    vector<pair<long long,int>> h(n),o(n);
    for(int i=0;i<n;i++){cin>>h[i].first;h[i].second=i;}
    for(int i=0;i<n;i++){cin>>o[i].first;o[i].second=i;}
    sort(h.begin(),h.end());
    sort(o.begin(),o.end());
    auto dist=[&](long long a,long long b)->long long{
        long long d=abs(a-b);
        return min(d,L-d);
    };
    long long best=LLONG_MAX;
    int bestk=0;
    for(int k=0;k<n;k++){
        long long cost=0;
        for(int i=0;i<n;i++) cost+=dist(h[i].first,o[(i+k)%n].first);
        if(cost<best){best=cost;bestk=k;}
    }
    cout<<best<<"\n";
    vector<int> ans(n);
    for(int i=0;i<n;i++) ans[h[i].second]=o[(i+bestk)%n].second+1;
    for(int i=0;i<n;i++) cout<<ans[i]<<" \n"[i==n-1];
}
