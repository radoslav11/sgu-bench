#include <bits/stdc++.h>
using namespace std;
// BIT
struct BIT { vector<int> t; int n; void init(int n){this->n=n;t.assign(n+1,0);} void upd(int i,int v){for(i++;i<=n;i+=i&-i)t[i]+=v;} int qry(int i){int s=0;for(i++;i>0;i-=i&-i)s+=t[i];return s;} };
int main(){
    int N,K; scanf("%d%d",&N,&K);
    vector<long long> X(N),Y(N),W(N),U(N),V(N);
    for(int i=0;i<N;i++){scanf("%lld%lld%lld",&X[i],&Y[i],&W[i]);U[i]=X[i]+Y[i];V[i]=X[i]-Y[i];}
    // collect v coords
    vector<long long> vc;
    for(int i=0;i<N;i++){vc.push_back(V[i]-W[i]);vc.push_back(V[i]+W[i]);vc.push_back(V[i]);}
    sort(vc.begin(),vc.end());vc.erase(unique(vc.begin(),vc.end()),vc.end());
    int M=vc.size();
    auto ci=[&](long long x){return (int)(lower_bound(vc.begin(),vc.end(),x)-vc.begin());};
    // events: (u_val, type, data)
    // type: 0=rect_start(add), 1=query, 2=rect_end(remove)
    vector<tuple<long long,int,int>> evs;
    for(int i=0;i<N;i++){evs.push_back({U[i]-W[i],0,i});evs.push_back({U[i]+W[i],2,i});evs.push_back({U[i],1,i});}
    sort(evs.begin(),evs.end());
    BIT bit; bit.init(M);
    vector<int> cnt(N,0);
    for(auto&[u,tp,i]:evs){
        if(tp==0){bit.upd(ci(V[i]-W[i]),1);if(ci(V[i]+W[i])+1<M)bit.upd(ci(V[i]+W[i])+1,-1);}
        else if(tp==2){bit.upd(ci(V[i]-W[i]),-1);if(ci(V[i]+W[i])+1<M)bit.upd(ci(V[i]+W[i])+1,1);}
        else{cnt[i]=bit.qry(ci(V[i]))-1;}
    }
    vector<int> res;
    for(int i=0;i<N;i++)if(cnt[i]>=K)res.push_back(i+1);
    printf("%d\n",(int)res.size());
    for(int i=0;i<(int)res.size();i++)printf("%d%c",res[i]," \n"[i+1==(int)res.size()]);
    if(res.empty())printf("\n");
}
