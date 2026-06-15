#include <bits/stdc++.h>
using namespace std;
typedef double ld;
struct P { ld x, y; };
vector<P> convexHull(vector<P> pts) {
    int n = pts.size();
    sort(pts.begin(), pts.end(), [](const P& a, const P& b){ return a.x < b.x || (a.x == b.x && a.y < b.y); });
    vector<P> h;
    for(int i = 0; i < n; i++) { while(h.size()>=2 && ((h.back().x-h[h.size()-2].x)*(pts[i].y-h[h.size()-2].y)-(h.back().y-h[h.size()-2].y)*(pts[i].x-h[h.size()-2].x)) <= 0) h.pop_back(); h.push_back(pts[i]); }
    int lo = h.size();
    for(int i = n-2; i >= 0; i--) { while((int)h.size()>lo && ((h.back().x-h[h.size()-2].x)*(pts[i].y-h[h.size()-2].y)-(h.back().y-h[h.size()-2].y)*(pts[i].x-h[h.size()-2].x)) <= 0) h.pop_back(); h.push_back(pts[i]); }
    h.pop_back();
    return h;
}
int main(){
    int n; scanf("%d",&n);
    vector<P> pts(n);
    for(int i=0;i<n;i++) scanf("%lf%lf",&pts[i].x,&pts[i].y);
    vector<P> poly = convexHull(pts);
    int m; scanf("%d",&m);
    while(m--){
        ld ax,ay,bx,by; scanf("%lf%lf%lf%lf",&ax,&ay,&bx,&by);
        ld tlo=0, thi=1;
        int sz=poly.size();
        for(int i=0;i<sz;i++){
            P pi=poly[i], pj=poly[(i+1)%sz];
            ld nx=-(pj.y-pi.y), ny=(pj.x-pi.x);
            ld d0=nx*(ax-pi.x)+ny*(ay-pi.y);
            ld dd=nx*(bx-ax)+ny*(by-ay);
            if(fabs(dd)<1e-12){ if(d0<0){tlo=2;break;} }
            else { ld t=-d0/dd; if(dd>0) tlo=max(tlo,t); else thi=min(thi,t); }
        }
        ld len=0;
        if(tlo<thi) len=sqrt((bx-ax)*(bx-ax)+(by-ay)*(by-ay))*(thi-tlo);
        printf("%.2f\n",len);
    }
}
