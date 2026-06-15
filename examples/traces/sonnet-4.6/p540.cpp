#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, s, vmin, vmax;
    cin >> n >> s >> vmin >> vmax;
    vector<int> x(n), r(n), g(n), d(n);
    for(int i=0;i<n;i++) cin>>x[i]>>r[i]>>g[i]>>d[i];
    
    vector<double> cands;
    cands.push_back(vmin); cands.push_back(vmax);
    for(int i=0;i<n;i++){
        int p = r[i]+g[i];
        double tmin = (double)x[i]/vmax, tmax = (double)x[i]/vmin;
        for(int k=0; k*p <= tmax+p; k++){
            double t1 = k*p + d[i];
            double t2 = k*p + d[i] + r[i];
            if(t1>0 && t1>=tmin-1e-9 && t1<=tmax+1e-9){ double v=x[i]/t1; if(v>=vmin-1e-9&&v<=vmax+1e-9) cands.push_back(v); }
            if(t2>0 && t2>=tmin-1e-9 && t2<=tmax+1e-9){ double v=x[i]/t2; if(v>=vmin-1e-9&&v<=vmax+1e-9) cands.push_back(v); }
        }
    }
    sort(cands.begin(),cands.end());
    cands.erase(unique(cands.begin(),cands.end()),cands.end());
    
    auto countRed=[&](double v)->int{ int cnt=0; for(int i=0;i<n;i++){ double t=x[i]/v; int p=r[i]+g[i]; double tm=fmod(t,p); if(tm<0)tm+=p; if(!(tm<d[i]+1e-9 || tm>=d[i]+r[i]-1e-9)) cnt++; } return cnt; };
    auto getSet=[&](double v)->vector<int>{ vector<int> res; for(int i=0;i<n;i++){ double t=x[i]/v; int p=r[i]+g[i]; double tm=fmod(t,p); if(!(tm<d[i]+1e-9||tm>=d[i]+r[i]-1e-9)) res.push_back(i+1); } return res; };
    
    int best=n+1; double bestv=-1;
    vector<double> toTest=cands;
    for(int i=0;i+1<(int)cands.size();i++) toTest.push_back((cands[i]+cands[i+1])/2);
    sort(toTest.begin(),toTest.end());
    for(double v:toTest){ if(v<vmin-1e-9||v>vmax+1e-9)continue; int c=countRed(v); if(c<best||(c==best&&v>bestv)){best=c;bestv=v;} }
    
    cout<<fixed<<setprecision(10)<<bestv<<"\n";
    auto res=getSet(bestv);
    cout<<res.size()<<"\n";
    for(int x:res) cout<<x<<" "; if(!res.empty())cout<<"\n";
}
