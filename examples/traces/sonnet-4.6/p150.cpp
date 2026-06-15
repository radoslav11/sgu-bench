#include <bits/stdc++.h>
using namespace std;

int main(){
    long long x1,y1,x2,y2,n;
    cin>>x1>>y1>>x2>>y2>>n;
    
    long long dx=x2-x1, dy=y2-y1;
    
    // Collect crossing times as fractions p/q where q=|dx| or |dy|
    // Store as (numerator, denominator) with denominator > 0
    // Use common denominator = |dx|*|dy| ... actually store as double? No.
    // Store fractions reduced, compare as cross multiply
    
    vector<pair<long long,long long>> times; // p/q in (0,1), q>0
    
    auto addCrossings = [&](long long from, long long to, long long delta){
        if(delta==0) return;
        long long lo=min(from,to), hi=max(from,to);
        // t = (k-from)/delta, k integer, t in (0,1)
        // if delta>0: k in (from, from+delta) integers
        long long kstart = lo+1, kend = hi;
        for(long long k=kstart;k<=kend;k++){
            long long num = (k-from); // delta>0 means from=lo
            if(delta<0) num=(from-k), delta=-delta; // fix sign
            // t = num/|delta|
            times.push_back({abs(k-from), abs(delta)});
            if(delta<0) delta=-delta;
        }
    };
    
    if(dx!=0){
        long long lo=min(x1,x2),hi=max(x1,x2);
        for(long long k=lo+1;k<=hi;k++){
            times.push_back({abs(k-x1),abs(dx)});
        }
    }
    if(dy!=0){
        long long lo=min(y1,y2),hi=max(y1,y2);
        for(long long k=lo+1;k<=hi;k++){
            times.push_back({abs(k-y1),abs(dy)});
        }
    }
    
    sort(times.begin(),times.end(),[](auto&a,auto&b){
        return a.first*b.second < b.first*a.second;
    });
    
    // Remove duplicates
    times.erase(unique(times.begin(),times.end(),[](auto&a,auto&b){
        return a.first*b.second==b.first*a.second;
    }),times.end());
    
    // Total cells = times.size()+1
    long long total=(long long)times.size()+1;
    if(n>total){cout<<"no solution";return 0;}
    
    double t;
    if(n==1) t=0.0;
    else t=(double)times[n-2].first/times[n-2].second;
    
    double px=x1+t*dx, py=y1+t*dy;
    // Move slightly forward
    double eps=1e-9;
    px+=eps*dx; py+=eps*dy;
    
    cout<<(long long)floor(px)<<" "<<(long long)floor(py);
}
