#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,k; cin>>n>>k;
    map<int,vector<int>> mp;
    for(int i=0;i<n;i++){int m,c;cin>>m>>c;mp[m].push_back(c);}
    vector<pair<int,int>> masses;
    for(auto&[m,v]:mp){sort(v.rbegin(),v.rend());masses.push_back({(int)v.size(),m});}
    sort(masses.rbegin(),masses.rend());
    if((int)masses.size()<k){cout<<"0 0\n";return 0;}
    int t=masses[k-1].first;
    vector<long long> vals;
    for(auto&[cnt,m]:masses){
        if(cnt>=t){
            auto&v=mp[m];
            long long s=0;for(int i=0;i<t;i++)s+=v[i];
            vals.push_back(s);
        }
    }
    sort(vals.rbegin(),vals.rend());
    long long total=0;for(int i=0;i<k;i++)total+=vals[i];
    cout<<t<<" "<<total<<"\n";
}
