#include <bits/stdc++.h>
using namespace std;

int main(){
    int n; cin>>n;
    vector<int> x(n),y(n);
    for(int i=0;i<n;i++) cin>>x[i]>>y[i];
    
    // Compute dp1[i] = LIS length ending at i (strictly increasing x and y)
    // Sort by x asc, y desc for strict
    vector<int> idx(n); iota(idx.begin(),idx.end(),0);
    sort(idx.begin(),idx.end(),[&](int a,int b){ return x[a]!=x[b]?x[a]<x[b]:y[a]>y[b]; });
    
    // Compress y
    vector<int> ys=y; sort(ys.begin(),ys.end()); ys.erase(unique(ys.begin(),ys.end()),ys.end());
    auto cy=[&](int v){ return lower_bound(ys.begin(),ys.end(),v)-ys.begin()+1; };
    int M=ys.size();
    vector<int> bit(M+2,0);
    auto upd=[&](int p,int v){ for(;p<=M;p+=p&-p) bit[p]=max(bit[p],v); };
    auto qry=[&](int p){ int r=0; for(;p>0;p-=p&-p) r=max(r,bit[p]); return r; };
    
    vector<int> dp1(n),dp2(n);
    for(int i:idx){ int c=cy(y[i]); dp1[i]=qry(c-1)+1; upd(c,dp1[i]); }
    
    fill(bit.begin(),bit.end(),0);
    sort(idx.begin(),idx.end(),[&](int a,int b){ return x[a]!=x[b]?x[a]>x[b]:y[a]<y[b]; });
    // For dp2, we want strictly decreasing y, so use M+1-cy(y[i])
    for(int i:idx){ int c=M+1-cy(y[i]); dp2[i]=qry(c-1)+1; upd(c,dp2[i]); }
    
    int L=0; for(int i=0;i<n;i++) L=max(L,dp1[i]+dp2[i]-1);
    
    vector<int> A,B;
    for(int i=0;i<n;i++) if(dp1[i]+dp2[i]-1==L) A.push_back(i+1);
    
    // B: for each level k in [1..L], count cities in A with dp1[i]==k; if exactly 1, add to B
    map<int,vector<int>> byLevel;
    for(int v:A) byLevel[dp1[v-1]].push_back(v);
    for(auto&[k,vec]:byLevel) if(vec.size()==1) B.push_back(vec[0]);
    sort(B.begin(),B.end());
    
    cout<<A.size(); for(int v:A) cout<<" "<<v; cout<<"\n";
    cout<<B.size(); for(int v:B) cout<<" "<<v; cout<<"\n";
}
