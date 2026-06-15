#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N,M,T;
    scanf("%lld %lld %lld",&N,&M,&T);
    vector<int> C(N);
    for(auto&c:C)scanf("%d",&c);
    vector<vector<char>> seat(N);
    for(int i=0;i<N;i++)seat[i].assign(C[i],0);
    struct G{long long Ti,Pi,Wi,Ei;};
    vector<G> g(M);
    for(int i=0;i<M;i++)scanf("%lld %lld %lld %lld",&g[i].Ti,&g[i].Pi,&g[i].Wi,&g[i].Ei);
    multiset<long long> times;
    for(int i=0;i<M;i++){times.insert(g[i].Ti); times.insert(g[i].Ti+g[i].Wi);}
    multimap<long long,array<int,3>> deps;
    deque<int> q;
    int arrIdx=0;
    double total=0;
    auto tryseat=[&](int gi,long long t)->bool{
        long long P=g[gi].Pi;
        int bestCounter=-1,bestStart=-1; long long bMin=-1,bMax=-1;
        for(int c=0;c<N;c++){
            int len=C[c];
            for(int s=0;s+P<=len;s++){
                bool ok=true;
                for(int k=0;k<P;k++)if(seat[c][s+k]){ok=false;break;}
                if(!ok)continue;
                long long SL,SR;
                if(s==0)SL=LLONG_MAX; else{int l=s-1,cnt=0;while(l>=0&&!seat[c][l]){cnt++;l--;}SL=cnt;}
                if(s+P==len)SR=LLONG_MAX; else{int r=s+P,cnt=0;while(r<len&&!seat[c][r]){cnt++;r++;}SR=cnt;}
                long long mn=min(SL,SR),mx=max(SL,SR);
                if(bestCounter==-1||mn>bMin||(mn==bMin&&mx>bMax)){
                    bestCounter=c;bestStart=s;bMin=mn;bMax=mx;
                }
            }
        }
        if(bestCounter==-1)return false;
        for(int k=0;k<P;k++)seat[bestCounter][bestStart+k]=1;
        deps.insert({t+g[gi].Ei,{bestCounter,bestStart,(int)P}});
        times.insert(t+g[gi].Ei);
        long long ti=t-g[gi].Ti;
        total += (double)(g[gi].Wi - ti)/(double)g[gi].Wi * P;
        return true;
    };
    while(!times.empty()){
        long long t=*times.begin();
        while(!times.empty()&&*times.begin()==t)times.erase(times.begin());
        auto rng=deps.equal_range(t);
        for(auto it=rng.first;it!=rng.second;++it){
            auto&a=it->second;
            for(int k=0;k<a[2];k++)seat[a[0]][a[1]+k]=0;
        }
        deps.erase(rng.first,rng.second);
        while(arrIdx<M&&g[arrIdx].Ti==t){q.push_back(arrIdx);arrIdx++;}
        while(!q.empty()){
            int gi=q.front();
            if(tryseat(gi,t)){q.pop_front();continue;}
            long long dl=g[gi].Ti+g[gi].Wi;
            if(t>=dl && dl<T){ total += -1.0*g[gi].Pi; q.pop_front(); continue;}
            break;
        }
    }
    long long tot=0;for(int i=0;i<M;i++)tot+=g[i].Pi;
    printf("%.16f\n",total/(double)tot);
}
