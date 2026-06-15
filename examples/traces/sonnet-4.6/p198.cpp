#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    while(cin>>n){
        vector<double>x(n),y(n),r(n);
        for(int i=0;i<n;i++) cin>>x[i]>>y[i]>>r[i];
        double sx,sy,sr;
        cin>>sx>>sy>>sr;
        // expand each island by sr
        // two expanded circles block if dist(c1,c2) < r1+r2+2*sr
        // ship trapped if expanded circles form closed barrier around (sx,sy)
        // Use: ship is trapped iff (sx,sy) is inside the union of expanded circles... no
        // Check: build graph, find if any cycle encloses the point
        // Simpler: check if point is enclosed by doing ray casting on the circle arrangement
        // For each pair connected, check if the "gate" between them is on a ray from ship
        // Standard: cast a ray from ship to infinity, count how many "blocking pairs" the ray crosses
        // Two islands block the ray if: the ray passes between them and dist < r1+r2+2R
        // Actually: for a ray from ship in direction +x, count islands whose expanded circle crosses the ray
        // but we need connected crossings... 
        // The ship is trapped iff for EVERY ray from the ship, the ray is blocked by connected islands
        // Easiest: cast ray upward (+y direction) from (sx,sy). The ray is blocked by island i if:
        // x[i] is within (r[i]+sr) of sx AND y[i]>sy (upper half)
        // Collect all such islands on the ray, sort by y, check if consecutive ones are connected (or if first one is reached from ground)
        // Actually the correct approach: 
        // The number of times the boundary of the union of expanded circles crosses the upward ray (with sign) = winding number
        // But union of circles boundary is complex.
        // Let me just do: ray from ship going up. For each island whose expanded circle intersects this ray (i.e., |x[i]-sx| < r[i]+sr and y[i]>sy), it contributes a crossing. If total crossings (counted with multiplicity based on connected components) is odd, trapped.
        // But overlapping circles on the ray need to be merged.
        // 
        // Intervals on the ray: each island i with |x[i]-sx|<=r[i]+sr contributes an interval of y values
        // Merge intervals, count how many merged intervals are above sy. If odd -> trapped? Not quite because need connectivity in 2D not just along ray.
        // 
        // I think the correct solution: 
        // Cast a ray. Islands that intersect the ray form intervals. Merge overlapping intervals (they're connected). Count merged intervals above ship. If odd => trapped.
        // This works because the winding number of the union boundary around the ship equals the number of times the ray crosses the boundary of the union.
        
        // ray: x = sx, y > sy (going up)
        // island i intersects ray if |x[i]-sx| <= r[i]+sr (use < for open, but touching allowed so use <=... hmm)
        // since touching is allowed (ship can touch), use strict < 
        // actually if dist between ship center and island boundary equals sr, ship just touches - allowed
        // so expanded circle: ship center can be AT distance r[i]+sr from island center
        // the expanded circle's boundary is at r[i]+sr
        // ship is blocked if it tries to cross INTO the expanded circle
        // ray crosses expanded circle i if |x[i]-sx| < r[i]+sr (strictly, since at equality the ray is tangent = touching = allowed)
        
        vector<pair<double,double>> intervals;
        for(int i=0;i<n;i++){
            double dx = x[i]-sx;
            double er = r[i]+sr;
            if(fabs(dx) < er - 1e-9){
                double dy = sqrt(er*er - dx*dx);
                double ylo = y[i]-dy;
                double yhi = y[i]+dy;
                // intersect with y > sy
                if(yhi > sy + 1e-9){
                    intervals.push_back({max(ylo, sy), yhi});
                }
            }
        }
        // merge intervals
        sort(intervals.begin(),intervals.end());
        int crossings = 0;
        double curLo = -1e18, curHi = -1e18;
        bool inInterval = false;
        // Actually just count merged intervals
        vector<pair<double,double>> merged;
        for(auto&p:intervals){
            if(merged.empty()||p.first > merged.back().second + 1e-9){
                merged.push_back(p);
            } else {
                merged.back().second = max(merged.back().second, p.second);
            }
        }
        crossings = merged.size();
        if(crossings % 2 == 1) cout<<"NO"<<endl;
        else cout<<"YES"<<endl;
    }
}
