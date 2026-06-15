#include <bits/stdc++.h>
using namespace std;

int numGaps;
vector<vector<int>> stk;
long long areaF[60005];

void seg_push(int node,int l,int r,int ql,int qr,int id){
    if(qr<l||r<ql) return;
    if(ql<=l&&r<=qr){ stk[node].push_back(id); return; }
    int mid=(l+r)>>1;
    seg_push(node*2,l,mid,ql,qr,id);
    seg_push(node*2+1,mid+1,r,ql,qr,id);
}
void seg_remove(int node,int l,int r,int ql,int qr){
    if(qr<l||r<ql) return;
    if(ql<=l&&r<=qr){ stk[node].pop_back(); return; }
    int mid=(l+r)>>1;
    seg_remove(node*2,l,mid,ql,qr);
    seg_remove(node*2+1,mid+1,r,ql,qr);
}
long long bestArea; int bestId;
void seg_query(int node,int l,int r,int pos){
    if(!stk[node].empty()){
        int id=stk[node].back();
        if(areaF[id]<bestArea){ bestArea=areaF[id]; bestId=id; }
    }
    if(l==r) return;
    int mid=(l+r)>>1;
    if(pos<=mid) seg_query(node*2,l,mid,pos);
    else seg_query(node*2+1,mid+1,r,pos);
}

int main(){
    int N; 
    scanf("%d",&N);
    long long W,H; scanf("%lld %lld",&W,&H);
    vector<long long> x1(N),x2(N),y1(N),y2(N);
    vector<long long> ys;
    for(int i=0;i<N;i++){
        long long a,b,c,d; scanf("%lld %lld %lld %lld",&a,&b,&c,&d);
        x1[i]=min(a,c); x2[i]=max(a,c);
        y1[i]=min(b,d); y2[i]=max(b,d);
        ys.push_back(y1[i]); ys.push_back(y2[i]);
        areaF[i]=(x2[i]-x1[i])*(y2[i]-y1[i]);
    }
    sort(ys.begin(),ys.end());
    ys.erase(unique(ys.begin(),ys.end()),ys.end());
    int m=ys.size();
    numGaps=m-1;
    auto idx=[&](long long v){ return (int)(lower_bound(ys.begin(),ys.end(),v)-ys.begin()); };
    
    stk.assign(4*max(1,numGaps), vector<int>());
    
    // events: (x, prio, id) prio: 0 remove,1 query,2 insert
    vector<array<long long,3>> ev;
    ev.reserve(3*N);
    for(int i=0;i<N;i++){
        ev.push_back({x2[i],0,(long long)i}); // remove
        ev.push_back({x1[i],1,(long long)i}); // query
        ev.push_back({x1[i],2,(long long)i}); // insert
    }
    sort(ev.begin(),ev.end(),[](const array<long long,3>&A,const array<long long,3>&B){
        if(A[0]!=B[0]) return A[0]<B[0];
        return A[1]<B[1];
    });
    
    vector<int> par(N,-1);
    for(auto&e:ev){
        int i=(int)e[2];
        int gl=idx(y1[i]);
        int gr=idx(y2[i])-1;
        if(e[1]==0){
            // remove
            seg_remove(1,0,numGaps-1,gl,gr);
        } else if(e[1]==1){
            // query at gap idx(y1[i])
            bestArea=LLONG_MAX; bestId=-1;
            seg_query(1,0,numGaps-1,gl);
            par[i]=bestId;
        } else {
            seg_push(1,0,numGaps-1,gl,gr,i);
        }
    }
    
    vector<long long> sub(N,0);
    long long rootSub=0;
    for(int i=0;i<N;i++){
        if(par[i]==-1) rootSub+=areaF[i];
        else sub[par[i]]+=areaF[i];
    }
    
    vector<long long> facets;
    facets.reserve(N+1);
    facets.push_back(W*H - rootSub);
    for(int i=0;i<N;i++){
        facets.push_back(areaF[i]-sub[i]);
    }
    sort(facets.begin(),facets.end());
    
    // output
    string out;
    out.reserve(facets.size()*12);
    char buf[32];
    for(size_t i=0;i<facets.size();i++){
        int len=sprintf(buf,"%lld",facets[i]);
        out.append(buf,len);
        out.push_back(i+1==facets.size()?'\n':' ');
    }
    fputs(out.c_str(),stdout);
    return 0;
}
