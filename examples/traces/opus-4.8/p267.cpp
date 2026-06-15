#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, K;
    scanf("%d %d", &N, &K);
    struct Pie{ long long cross; bool good; };
    vector<Pie> pies(N);
    for(int i=0;i<N;i++){
        long long x[4],y[4];
        long long sx=0, sy=0;
        for(int j=0;j<4;j++){
            scanf("%lld %lld",&x[j],&y[j]);
            sx+=x[j]; sy+=y[j];
        }
        long long ax,ay,bx,by;
        scanf("%lld %lld %lld %lld",&ax,&ay,&bx,&by);
        // area: find opposite corner of point0
        // farthest from point0
        int opp=1;
        long long best=-1;
        for(int j=1;j<4;j++){
            long long dx=x[j]-x[0], dy=y[j]-y[0];
            long long d=dx*dx+dy*dy;
            if(d>best){best=d;opp=j;}
        }
        // other two indices
        int o1=-1,o2=-1;
        for(int j=1;j<4;j++) if(j!=opp){ if(o1<0)o1=j; else o2=j; }
        long long ACx = x[opp]-x[0], ACy = y[opp]-y[0];
        long long BDx = x[o2]-x[o1], BDy = y[o2]-y[o1];
        long long crossDiag = ACx*BDy - ACy*BDx;
        long long crossArea = llabs(crossDiag); // this = 2 * area
        // good check
        // center scaled by 4: (sx,sy)
        long long ux = 4*ax - sx, uy = 4*ay - sy;
        long long vx = 4*bx - sx, vy = 4*by - sy;
        bool good;
        if((ux==0&&uy==0)||(vx==0&&vy==0)){
            good=false;
        } else {
            long long cr = ux*vy - uy*vx;
            long long dt = ux*vx + uy*vy;
            if(cr==0 && dt>0) good=false; // same direction
            else good=true;
        }
        pies[i].cross=crossArea;
        pies[i].good=good;
    }
    if(K==0){
        printf("0.000 0.000\n");
        return 0;
    }
    sort(pies.begin(), pies.end(), [](const Pie&a, const Pie&b){
        return a.cross > b.cross;
    });
    long long threshold = pies[K-1].cross;
    // strictly greater
    double eaten_strict = 0.0;
    int cnt_greater=0;
    int good_in_tie=0, bad_in_tie=0;
    for(int i=0;i<N;i++){
        if(pies[i].cross > threshold){
            cnt_greater++;
            if(pies[i].good) eaten_strict += pies[i].cross/4.0;
        } else if(pies[i].cross == threshold){
            if(pies[i].good) good_in_tie++;
            else bad_in_tie++;
        }
    }
    int remaining = K - cnt_greater;
    int optGood = min(remaining, good_in_tie);
    int pesGood = max(0, remaining - bad_in_tie);
    double opt = eaten_strict + optGood*(threshold/4.0);
    double pes = eaten_strict + pesGood*(threshold/4.0);
    printf("%.3f %.3f\n", pes, opt);
    return 0;
}
