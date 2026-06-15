#include <bits/stdc++.h>
using namespace std;

double h,d,m;
double g(double t){
    double Bx=m-t, Cx=m+t;
    double BD=d-Bx;
    double DC=Cx-d;
    return BD*sqrt(Cx*Cx+h*h)-DC*sqrt(Bx*Bx+h*h);
}

int main(){
    double H,BD,BM;
    if(!(cin>>H>>BD>>BM)) return 0;
    h=H;
    // need H<=BD<=BM
    if(H>BD+1e-9 || BD>BM+1e-9){
        printf("NO\n");
        return 0;
    }
    // case bd==bm
    if(fabs(BD-BM)<1e-9){
        // requires all equal
        if(fabs(BD-H)<1e-9){
            // isoceles, altitude=bisector=median=H, midpoint at (0,0)
            double t=H>0?H:1.0;
            printf("YES\n");
            printf("%.8f %.8f\n",0.0,H);
            printf("%.8f %.8f\n",-t,0.0);
            printf("%.8f %.8f\n",t,0.0);
        } else {
            printf("NO\n");
        }
        return 0;
    }
    // now BD<BM strictly
    d=sqrt(max(0.0,BD*BD-H*H));
    m=sqrt(max(0.0,BM*BM-H*H));
    if(m<=d+1e-12){
        // shouldn't generally happen but safety
        printf("NO\n");
        return 0;
    }
    double lo=m-d; // g(lo)<=0
    double hi=lo;
    // find hi with g>0
    bool found=false;
    double step=max(1.0,m);
    for(int it=0; it<200; it++){
        hi+= step;
        step*=1.5;
        if(g(hi)>0){found=true;break;}
        if(hi>2e7) break;
    }
    if(!found){
        printf("NO\n");
        return 0;
    }
    // ensure g(lo)<=0
    if(g(lo)>0){
        // try lower lo
        printf("NO\n");
        return 0;
    }
    for(int it=0; it<200; it++){
        double mid=(lo+hi)/2;
        if(g(mid)<=0) lo=mid; else hi=mid;
    }
    double t=(lo+hi)/2;
    if(t<=1e-9){
        printf("NO\n");
        return 0;
    }
    double Bx=m-t, Cx=m+t;
    // check coords bound
    if(fabs(Bx)>1e8||fabs(Cx)>1e8||fabs(h)>1e8){
        printf("NO\n");
        return 0;
    }
    printf("YES\n");
    printf("%.8f %.8f\n",0.0,h);
    printf("%.8f %.8f\n",Bx,0.0);
    printf("%.8f %.8f\n",Cx,0.0);
    return 0;
}
