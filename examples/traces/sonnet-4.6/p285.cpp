#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    double p[13];
    for(int i=0;i<12;i++) cin>>p[i];
    double sum=0;
    for(int i=0;i<N;i++){double x;cin>>x;sum+=x;}
    p[12]=(N>0)?sum/N:0.5;
    map<tuple<int,int,int>,double> dp;
    dp[{(1<<13)-1,0,0}]=1.0;
    double ans[12]={};
    queue<tuple<int,int,int>> q;
    q.push({(1<<13)-1,0,0});
    while(!q.empty()){
        auto [mask,c,v]=q.front();q.pop();
        double prob=dp[{mask,c,v}];
        if(prob<1e-18)continue;
        for(int s=1;s<=13;s++){
            int cur=s;
            while(!(mask&(1<<(cur-1)))){cur--;if(cur==0)cur=13;}
            int sector=cur;
            int newmask=mask^(1<<(sector-1));
            double pc=p[sector-1];
            for(int win=0;win<2;win++){
                double wp=win?pc:(1-pc);
                int nc=c+(win?1:0),nv=v+(win?0:1);
                double np=prob/13.0*wp;
                if(nc==6||nv==6){int idx=nc==6?nv:6+(6-nc);ans[idx]+=np;}
                else{auto key=make_tuple(newmask,nc,nv);if(dp.find(key)==dp.end())q.push(key);dp[key]+=np;}
            }
        }
    }
    cout<<fixed<<setprecision(3);
    for(int i=0;i<12;i++)cout<<ans[i]<<"\n";
}
