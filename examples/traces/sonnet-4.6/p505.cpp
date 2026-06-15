#include <bits/stdc++.h>
using namespace std;

int bit[1000002];
int BN;
void upd(int i,int v){for(i++;i<=BN;i+=i&-i)bit[i]+=v;}
int qry(int i){int s=0;for(i++;i>0;i-=i&-i)s+=bit[i];return s;}
int qr(int l,int r){if(l>r)return 0;return qry(r)-(l?qry(l-1):0);}

int prefixLB(const vector<int>&idx, const vector<string>&g, const string&p){
    int lo=0,hi=(int)idx.size();
    while(lo<hi){
        int mid=(lo+hi)/2;
        if(g[idx[mid]]<p) lo=mid+1; else hi=mid;
    }
    return lo;
}
int prefixUB(const vector<int>&idx, const vector<string>&g, const string&p){
    int lo=0,hi=(int)idx.size();
    while(lo<hi){
        int mid=(lo+hi)/2;
        const string&s=g[idx[mid]];
        bool past;
        if((int)s.size()<(int)p.size()){
            past=(s.compare(0,s.size(),p,0,s.size())>0);
        } else {
            past=(s.compare(0,p.size(),p)>0);
        }
        if(past) hi=mid; else lo=mid+1;
    }
    return lo;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n; cin>>n;
    vector<string> g(n);
    for(int i=0;i<n;i++) cin>>g[i];
    vector<string> rg(n);
    for(int i=0;i<n;i++){rg[i]=g[i];reverse(rg[i].begin(),rg[i].end());}
    
    vector<int> pidx(n),sidx(n);
    iota(pidx.begin(),pidx.end(),0);
    iota(sidx.begin(),sidx.end(),0);
    sort(pidx.begin(),pidx.end(),[&](int a,int b){return g[a]<g[b];});
    sort(sidx.begin(),sidx.end(),[&](int a,int b){return rg[a]<rg[b];});
    
    vector<int> sr(n);
    for(int i=0;i<n;i++) sr[sidx[i]]=i;
    
    BN=n;
    
    int m; cin>>m;
    vector<int> ql1(m),qr1(m),ql2(m),qr2(m);
    for(int i=0;i<m;i++){
        string p,s; cin>>p>>s;
        string rs=s; reverse(rs.begin(),rs.end());
        ql1[i]=prefixLB(pidx,g,p);
        qr1[i]=prefixUB(pidx,g,p)-1;
        ql2[i]=prefixLB(sidx,rg,rs);
        qr2[i]=prefixUB(sidx,rg,rs)-1;
    }
    
    // events: (position in pidx, query_id, sign, l2, r2)
    struct Ev{int pos,qi,sign,l2,r2;};
    vector<Ev> evs;
    for(int i=0;i<m;i++){
        if(ql1[i]>qr1[i]||ql2[i]>qr2[i]) continue;
        evs.push_back({qr1[i],i,1,ql2[i],qr2[i]});
        if(ql1[i]>0) evs.push_back({ql1[i]-1,i,-1,ql2[i],qr2[i]});
    }
    sort(evs.begin(),evs.end(),[](const Ev&a,const Ev&b){return a.pos<b.pos;});
    
    vector<int> ans(m,0);
    int pos=-1;
    for(auto&e:evs){
        while(pos<e.pos){
            pos++;
            upd(sr[pidx[pos]],1);
        }
        ans[e.qi]+=e.sign*qr(e.l2,e.r2);
    }
    
    for(int i=0;i<m;i++) cout<<ans[i]<<"\n";
}
