#include <bits/stdc++.h>
using namespace std;

double gB1,gT1,gB2,gT2,gL,gDs,gDf,gG;

double jumpSpeedSq(double D, double d, double ylow, double yhigh, double g){
    // projectile from (0,0) to (D,0), at horizontal d height must be in [ylow,yhigh]
    if(d<=0 || d>=D) return INFINITY;
    double c = (g*D/2.0)*(g*D/2.0);          // (g D /2)^2
    double base = g*d*(D-d)/2.0;
    double A2 = base / yhigh;  // lower bound on vx^2
    double B2 = base / ylow;   // upper bound on vx^2
    double u = sqrt(c);
    if(u < A2) u = A2;
    if(u > B2) u = B2;
    if(u <= 0) return INFINITY;
    return u + c/u;            // speed^2
}

double F(double p){
    double s1 = jumpSpeedSq(gDs+p, gDs, gB1, gT1, gG);
    double s2 = jumpSpeedSq((gL-p)+gDf, gL-p, gB2, gT2, gG);
    return max(s1,s2);
}

int main(){
    double b1,t1,b2,t2,l,ds,df,g;
    cout.setf(ios::fixed); cout<<setprecision(4);
    while(cin>>b1>>t1>>b2>>t2>>l>>ds>>df>>g){
        gB1=b1;gT1=t1;gB2=b2;gT2=t2;gL=l;gDs=ds;gDf=df;gG=g;
        
        int N=2000;
        double best=INFINITY; double bp=l/2.0;
        int bestIdx=-1;
        vector<double> ps(N+1), vs(N+1);
        for(int i=0;i<=N;i++){
            double p = l*i/(double)N;
            ps[i]=p;
            double f=F(p);
            vs[i]=f;
            if(f<best){best=f;bp=p;bestIdx=i;}
        }
        // golden section refine in bracket around bestIdx
        int lo=max(0,bestIdx-1), hi=min(N,bestIdx+1);
        double a=ps[lo], bb=ps[hi];
        const double gr=(sqrt(5.0)-1.0)/2.0;
        double x1=bb-gr*(bb-a);
        double x2=a+gr*(bb-a);
        double f1=F(x1), f2=F(x2);
        for(int it=0;it<200;it++){
            if(f1<f2){
                bb=x2; x2=x1; f2=f1;
                x1=bb-gr*(bb-a); f1=F(x1);
            } else {
                a=x1; x1=x2; f1=f2;
                x2=a+gr*(bb-a); f2=F(x2);
            }
        }
        double res=min(min(f1,f2),best);
        if(!isfinite(res) || res>1e18){
            cout<<-1<<"\n";
        } else {
            cout<<sqrt(res)<<"\n";
        }
    }
    return 0;
}
