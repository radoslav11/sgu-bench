#include <bits/stdc++.h>
using namespace std;
typedef double ld;
struct P { ld x, y; };
int main(){
    int n;
    scanf("%d",&n);
    vector<P> pts(n);
    for(int i=0;i<n;i++) scanf("%lf %lf",&pts[i].x,&pts[i].y);
    sort(pts.begin(),pts.end(),[](const P&a,const P&b){
        return a.x<b.x || (a.x==b.x && a.y<b.y);
    });
    auto cross=[](const P&O,const P&A,const P&B){
        return (A.x-O.x)*(B.y-O.y)-(A.y-O.y)*(B.x-O.x);
    };
    int N=n;
    vector<P> hull(2*N);
    int k=0;
    for(int i=0;i<N;i++){
        while(k>=2 && cross(hull[k-2],hull[k-1],pts[i])<=0) k--;
        hull[k++]=pts[i];
    }
    int lower=k+1;
    for(int i=N-2;i>=0;i--){
        while(k>=lower && cross(hull[k-2],hull[k-1],pts[i])<=0) k--;
        hull[k++]=pts[i];
    }
    hull.resize(k-1);
    int H=hull.size();
    int m;
    scanf("%d",&m);
    while(m--){
        ld ax,ay,bx,by;
        scanf("%lf %lf %lf %lf",&ax,&ay,&bx,&by);
        ld dx=bx-ax, dy=by-ay;
        ld tlo=0, thi=1;
        bool valid=true;
        for(int i=0;i<H && valid;i++){
            P V=hull[i], W=hull[(i+1)%H];
            ld ex=W.x-V.x, ey=W.y-V.y;
            ld c0 = ex*(ay-V.y) - ey*(ax-V.x);
            ld c1 = ex*dy - ey*dx;
            if(fabs(c1)<1e-12){
                if(c0 < 0) valid=false;
            } else {
                ld tb = -c0/c1;
                if(c1>0){ if(tb>tlo) tlo=tb; }
                else { if(tb<thi) thi=tb; }
            }
        }
        ld len=0;
        if(valid && thi>tlo) len=sqrt(dx*dx+dy*dy)*(thi-tlo);
        printf("%.2f\n",len);
    }
    return 0;
}
