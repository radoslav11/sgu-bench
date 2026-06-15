#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
double R;

struct P { double x,y; };

vector<P> convexHull(vector<pair<ll,ll>> pts){
    sort(pts.begin(),pts.end());
    pts.erase(unique(pts.begin(),pts.end()),pts.end());
    int n=pts.size();
    if(n<=1){
        vector<P> r; for(auto&p:pts) r.push_back({(double)p.first,(double)p.second}); return r;
    }
    vector<pair<ll,ll>> h(2*n);
    int k=0;
    auto cross=[&](pair<ll,ll>&O,pair<ll,ll>&A,pair<ll,ll>&B)->ll{
        return (A.first-O.first)*(B.second-O.second)-(A.second-O.second)*(B.first-O.first);
    };
    for(int i=0;i<n;i++){
        while(k>=2 && cross(h[k-2],h[k-1],pts[i])<=0) k--;
        h[k++]=pts[i];
    }
    int lower=k+1;
    for(int i=n-2;i>=0;i--){
        while(k>=lower && cross(h[k-2],h[k-1],pts[i])<=0) k--;
        h[k++]=pts[i];
    }
    h.resize(k-1);
    vector<P> res;
    for(auto&p:h) res.push_back({(double)p.first,(double)p.second});
    return res;
}

double capArea(double c){
    if(c<=-R) return M_PI*R*R;
    if(c>=R) return 0.0;
    return R*R*acos(c/R) - c*sqrt(R*R - c*c);
}

vector<P> Jh, Sh;

void evalAB(double th, double &A, double &B){
    double cx=cos(th), cy=sin(th);
    // A = min over J, B = max over S
    A=1e30; B=-1e30;
    for(auto&p:Jh){ double d=cx*p.x+cy*p.y; if(d<A)A=d; }
    for(auto&p:Sh){ double d=cx*p.x+cy*p.y; if(d>B)B=d; }
}

// for max area: minimize B (push line toward See), need feasible B<=A
// objective area = capArea(clamp(B)), maximize
double evalMax(double th){
    double A,B; evalAB(th,A,B);
    if(B>A+1e-9) return -1.0; // infeasible
    return capArea(B);
}
// for min area: maximize A, need feasible
double evalMin(double th){
    double A,B; evalAB(th,A,B);
    if(B>A+1e-9) return 1e30;
    return capArea(A);
}

int main(){
    long long ri;
    if(scanf("%lld",&ri)!=1) return 0;
    R=(double)ri;
    int n; scanf("%d",&n);
    vector<pair<ll,ll>> J(n);
    for(int i=0;i<n;i++) scanf("%lld %lld",&J[i].first,&J[i].second);
    int m; scanf("%d",&m);
    vector<pair<ll,ll>> S(m);
    for(int i=0;i<m;i++) scanf("%lld %lld",&S[i].first,&S[i].second);
    
    Jh=convexHull(J);
    Sh=convexHull(S);
    
    int samples=400000;
    double bestMaxArea=-1.0, bestMaxTh=0;
    double bestMinArea=1e30, bestMinTh=0;
    
    for(int i=0;i<samples;i++){
        double th=2.0*M_PI*i/samples;
        double A,B; evalAB(th,A,B);
        if(B>A+1e-9) continue;
        double maxA=capArea(B);
        if(maxA>bestMaxArea){ bestMaxArea=maxA; bestMaxTh=th; }
        double minA=capArea(A);
        if(minA<bestMinArea){ bestMinArea=minA; bestMinTh=th; }
    }
    
    double step=2.0*M_PI/samples;
    // golden section refine for max area (maximize evalMax)
    {
        double lo=bestMaxTh-2*step, hi=bestMaxTh+2*step;
        double gr=(sqrt(5.0)-1)/2;
        double c=hi-gr*(hi-lo), d=lo+gr*(hi-lo);
        double fc=evalMax(c), fd=evalMax(d);
        for(int it=0;it<200;it++){
            if(fc<fd){ lo=c; c=d; fc=fd; d=lo+gr*(hi-lo); fd=evalMax(d);}    
            else { hi=d; d=c; fd=fc; c=hi-gr*(hi-lo); fc=evalMax(c);}    
        }
        double th=(lo+hi)/2;
        double v=evalMax(th);
        if(v>bestMaxArea) bestMaxArea=v;
        if(fc>bestMaxArea) bestMaxArea=fc;
        if(fd>bestMaxArea) bestMaxArea=fd;
    }
    // golden section refine for min area (minimize evalMin)
    {
        double lo=bestMinTh-2*step, hi=bestMinTh+2*step;
        double gr=(sqrt(5.0)-1)/2;
        double c=hi-gr*(hi-lo), d=lo+gr*(hi-lo);
        double fc=evalMin(c), fd=evalMin(d);
        for(int it=0;it<200;it++){
            if(fc<fd){ hi=d; d=c; fd=fc; c=hi-gr*(hi-lo); fc=evalMin(c);}    
            else { lo=c; c=d; fc=fd; d=lo+gr*(hi-lo); fd=evalMin(d);}    
        }
        double th=(lo+hi)/2;
        double v=evalMin(th);
        if(v<bestMinArea) bestMinArea=v;
        if(fc<bestMinArea) bestMinArea=fc;
        if(fd<bestMinArea) bestMinArea=fd;
    }
    
    printf("%.15f %.15f\n", bestMinArea, bestMaxArea);
    return 0;
}
