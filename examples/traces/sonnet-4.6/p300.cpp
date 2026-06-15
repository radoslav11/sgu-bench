#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main(){
    int n;
    cin >> n;
    vector<ll> x(n), y(n);
    for(int i=0;i<n;i++) cin>>x[i]>>y[i];
    
    int m = n-1;
    vector<double> arc(n, 0);
    for(int i=1;i<n;i++)
        arc[i] = arc[i-1] + abs(x[i]-x[i-1]) + abs(y[i]-y[i-1]);
    
    double ans = arc[n-1];
    
    for(int i=0;i<m;i++){
        for(int j=i+2;j<m;j++){
            ll x1=x[i],y1=y[i],x2=x[i+1],y2=y[i+1];
            ll x3=x[j],y3=y[j],x4=x[j+1],y4=y[j+1];
            ll dx1=x2-x1,dy1=y2-y1;
            ll dx2=x4-x3,dy2=y4-y3;
            ll det=dx1*dy2-dy1*dx2;
            if(det==0){
                ll cross = (x3-x1)*dy1 - (y3-y1)*dx1;
                if(cross != 0) continue;
                if(dx1 != 0){
                    ll lo = max(min(x1,x2), min(x3,x4));
                    ll hi = min(max(x1,x2), max(x3,x4));
                    if(lo > hi) continue;
                    for(ll px : {lo, hi}){
                        double ai = arc[i] + (double)abs(px-x1);
                        double aj = arc[j] + (double)abs(px-x3);
                        if(ai > aj) swap(ai,aj);
                        ans = min(ans, aj - ai);
                    }
                } else {
                    ll lo = max(min(y1,y2), min(y3,y4));
                    ll hi = min(max(y1,y2), max(y3,y4));
                    if(lo > hi) continue;
                    for(ll py : {lo, hi}){
                        double ai = arc[i] + (double)abs(py-y1);
                        double aj = arc[j] + (double)abs(py-y3);
                        if(ai > aj) swap(ai,aj);
                        ans = min(ans, aj - ai);
                    }
                }
            } else {
                ll num_t = (x3-x1)*dy2 - (y3-y1)*dx2;
                ll num_s = (x3-x1)*dy1 - (y3-y1)*dx1;
                ll lo_t = min(0LL,det), hi_t = max(0LL,det);
                ll lo_s = min(0LL,det), hi_s = max(0LL,det);
                if(num_t < lo_t || num_t > hi_t) continue;
                if(num_s < lo_s || num_s > hi_s) continue;
                double t = (double)num_t / det;
                double s = (double)num_s / det;
                double len_i = sqrt((double)(dx1*dx1+dy1*dy1));
                double len_j = sqrt((double)(dx2*dx2+dy2*dy2));
                double a = arc[i] + t * len_i;
                double b = arc[j] + s * len_j;
                if(a > b) swap(a, b);
                ans = min(ans, b - a);
            }
        }
    }
    cout << (ll)round(ans) << endl;
}
