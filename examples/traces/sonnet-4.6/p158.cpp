#include <bits/stdc++.h>
using namespace std;
int main(){
    int L,M,N;
    cin>>L>>M;
    vector<double>P(M);
    for(auto&x:P)cin>>x;
    cin>>N;
    vector<double>D(N);D[0]=0;
    for(int i=1;i<N;i++)cin>>D[i];
    double Smax=L-D[N-1];
    vector<double>cands;
    cands.push_back(0);cands.push_back(Smax);
    for(int i=0;i<M;i++){
        vector<double>c(N);
        for(int j=0;j<N;j++)c[j]=P[i]-D[j];
        sort(c.begin(),c.end());
        for(int j=0;j+1<N;j++)cands.push_back((c[j]+c[j+1])/2.0);
        for(int j=0;j<N;j++)cands.push_back(c[j]);
    }
    double bestS=0,bestVal=-1e18;
    for(double s:cands){
        if(s<0||s>Smax)continue;
        double val=0;
        for(int i=0;i<M;i++){
            double mn=1e18;
            for(int j=0;j<N;j++)mn=min(mn,fabs(D[j]+s-P[i]));
            val+=mn;
        }
        if(val>bestVal){bestVal=val;bestS=s;}
    }
    cout<<fixed<<setprecision(1)<<bestS<<" "<<bestVal<<endl;
}
