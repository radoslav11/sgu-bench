#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; double A,B;
    scanf("%d %lf %lf",&N,&A,&B);
    vector<double> slope, inter;
    double y1min=0; bool infeasible=false;
    for(int i=0;i<N;i++){
        double a,b,c; scanf("%lf %lf %lf",&a,&b,&c);
        if(c<=0) continue;
        if(b>0){ slope.push_back(-a/b); inter.push_back(c/b);}
        else { if(a>0) y1min=max(y1min,c/a); else infeasible=true; }
    }
    if(infeasible){ printf("inf\n"); return 0;}
    slope.push_back(0); inter.push_back(0);
    int n=slope.size();
    vector<int> idx(n); for(int i=0;i<n;i++) idx[i]=i;
    sort(idx.begin(),idx.end(),[&](int x,int y){ if(slope[x]!=slope[y])return slope[x]<slope[y]; return inter[x]>inter[y];});
    vector<double> sl,in;
    for(int k=0;k<n;k++){
        double s=slope[idx[k]], c=inter[idx[k]];
        if(!sl.empty() && sl.back()==s) continue;
        while(sl.size()>=2){
            int mm=sl.size();
            double x1=(in[mm-1]-c)/(s-sl[mm-1]);
            double x2=(in[mm-2]-in[mm-1])/(sl[mm-1]-sl[mm-2]);
            if(x1<=x2){ sl.pop_back(); in.pop_back(); } else break;
        }
        sl.push_back(s); in.push_back(c);
    }
    int m=sl.size();
    vector<double> bp; bp.push_back(y1min);
    for(int i=0;i+1<m;i++){
        double x=(in[i+1]-in[i])/(sl[i]-sl[i+1]);
        if(x>y1min) bp.push_back(x);
    }
    double best=1e18;
    auto envY2=[&](double y1)->double{
        double v=-1e18;
        for(int i=0;i<m;i++) v=max(v, sl[i]*y1+in[i]);
        return max(v,0.0);
    };
    for(double x:bp){
        double y2=envY2(x);
        best=min(best, A*x+B*y2);
    }
    printf("%.6f\n", best);
    return 0;
}
