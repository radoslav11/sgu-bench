#include <bits/stdc++.h>
using namespace std;

int main(){
    int N,M;
    cin>>N>>M;
    vector<array<int,3>> elems;
    for(int a=0;a<N;a++)for(int b=0;b<M;b++) elems.push_back({0,a,b});
    if(N==M){
        for(int a=0;a<N;a++)for(int b=0;b<M;b++) elems.push_back({1,a,b});
    }
    auto idx=[&](int r,int c){return r*M+c;};
    vector<long long> sum(1,0);
    const long long BASE=1000000000000000000LL;
    auto addPow2=[&](int e){
        vector<long long> p(1,1);
        for(int i=0;i<e;i++){
            long long carry=0;
            for(auto&d:p){ d=d*2+carry; carry=d/BASE; d%=BASE;}
            if(carry)p.push_back(carry);
        }
        while(sum.size()<p.size())sum.push_back(0);
        long long carry=0;
        for(size_t i=0;i<p.size();i++){ sum[i]+=p[i]+carry; carry=sum[i]/BASE; sum[i]%=BASE;}
        size_t i=p.size();
        while(carry){ if(i>=sum.size())sum.push_back(0); sum[i]+=carry; carry=sum[i]/BASE; sum[i]%=BASE; i++;}
    };
    int total=N*M;
    for(auto&e:elems){
        vector<char> vis(total,0);
        int cycles=0;
        for(int s=0;s<total;s++){
            if(vis[s])continue;
            cycles++;
            int cur=s;
            while(!vis[cur]){
                vis[cur]=1;
                int r=cur/M, c=cur%M;
                int nr,nc;
                if(e[0]==0){ nr=(r+e[1])%N; nc=(c+e[2])%M;}
                else { nr=(c+e[1])%N; nc=(r+e[2])%M;}
                cur=idx(nr,nc);
            }
        }
        addPow2(cycles);
    }
    long long G=elems.size();
    vector<long long> res(sum.size(),0);
    long long rem=0;
    for(int i=(int)sum.size()-1;i>=0;i--){
        __int128 cur=(__int128)rem*BASE+sum[i];
        res[i]=(long long)(cur/G);
        rem=(long long)(cur%G);
    }
    while(res.size()>1 && res.back()==0)res.pop_back();
    string out;
    char buf[32];
    sprintf(buf,"%lld",res.back());
    out+=buf;
    for(int i=(int)res.size()-2;i>=0;i--){ sprintf(buf,"%018lld",res[i]); out+=buf;}
    cout<<out<<"\n";
}
