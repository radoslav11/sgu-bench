#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main(){
    int n,m; ll d;
    cin>>n>>m>>d;
    vector<ll>p(n),q(m);
    for(auto&x:p)cin>>x;
    for(auto&x:q)cin>>x;
    // candidate t1 values: for each gap in p, t = gap/2 (floor), and gap/2+1? 
    // Actually breakpoints: number of groups changes when 2*t crosses a gap value
    // groups = 1 + #{gaps > 2t}
    // As t increases, groups decrease at t = floor(gap/2) for each gap (when 2t >= gap, i.e., t >= ceil(gap/2))
    // So candidate t1: 1, and for each gap g: ceil(g/2), and also values just below: floor((g-1)/2)+1 = ceil(g/2)
    // Distinct number of openings: we want minimum total openings
    // Enumerate all O(n) candidate t1 values (the gap/2 rounded values)
    
    auto getGroups=[](vector<ll>&v, ll t)->vector<pair<ll,ll>>{
        vector<pair<ll,ll>>grps;
        if(v.empty())return grps;
        ll s=v[0],e=v[0];
        for(int i=1;i<(int)v.size();i++){
            if(v[i]-v[i-1]<=2*t) e=v[i];
            else{grps.push_back({s-t,e+t});s=v[i];e=v[i];}
        }
        grps.push_back({s-t,e+t});
        return grps;
    };
    
    auto check=[&](ll t1,ll t2)->bool{
        auto g1=getGroups(p,t1);
        auto g2=getGroups(q,t2);
        for(auto&a:g1)for(auto&b:g2){
            ll ol=min(a.second,b.second)-max(a.first,b.first);
            if(ol>d)return false;
        }
        return true;
    };
    
    // Candidate t values
    vector<ll>ct1,ct2;
    ct1.push_back(1);
    for(int i=0;i+1<n;i++){ll g=p[i+1]-p[i];ct1.push_back(g/2);ct1.push_back((g+1)/2);}
    ct1.push_back((ll)2e9);
    ct2.push_back(1);
    for(int i=0;i+1<m;i++){ll g=q[i+1]-q[i];ct2.push_back(g/2);ct2.push_back((g+1)/2);}
    ct2.push_back((ll)2e9);
    
    sort(ct1.begin(),ct1.end());ct1.erase(unique(ct1.begin(),ct1.end()),ct1.end());
    sort(ct2.begin(),ct2.end());ct2.erase(unique(ct2.begin(),ct2.end()),ct2.end());
    // Remove t<1
    ct1.erase(remove_if(ct1.begin(),ct1.end(),[](ll x){return x<1;}),ct1.end());
    ct2.erase(remove_if(ct2.begin(),ct2.end(),[](ll x){return x<1;}),ct2.end());
    
    // For minimum total openings: larger t = fewer openings
    // So we want to find pair with minimum openings
    // Enumerate in order of decreasing t1+t2? 
    // Better: for each t1 candidate (sorted desc), find largest t2 that satisfies constraint
    
    int bestTotal=INT_MAX;
    ll best1=-1,best2=-1;
    
    // precompute openings for each candidate
    auto numGroups=[&](vector<ll>&v,ll t)->int{
        if(v.empty())return 0;
        int c=1;
        for(int i=1;i<(int)v.size();i++)if(v[i]-v[i-1]>2*t)c++;
        return c;
    };
    
    for(ll t1:ct1){
        int g1=numGroups(p,t1);
        if(g1+1>=bestTotal)continue; // even with 1 group for door2 won't improve
        for(ll t2:ct2){
            int g2=numGroups(q,t2);
            if(g1+g2>=bestTotal)continue;
            if(check(t1,t2)){
                if(g1+g2<bestTotal){
                    bestTotal=g1+g2;
                    best1=t1;best2=t2;
                }
            }
        }
    }
    
    if(best1==-1)cout<<"No solution";
    else cout<<best1<<" "<<best2;
}
