#include <bits/stdc++.h>
using namespace std;

int main(){
    int n; double s, vmin, vmax;
    // read as int then convert
    int s_i, vmin_i, vmax_i;
    scanf("%d %d %d %d", &n, &s_i, &vmin_i, &vmax_i);
    s=s_i; vmin=vmin_i; vmax=vmax_i;
    
    vector<double> VL, VR;
    vector<int> LID;
    VL.reserve(1<<20); VR.reserve(1<<20); LID.reserve(1<<20);
    
    for(int i=0;i<n;i++){
        int xi,ri,gi,di;
        scanf("%d %d %d %d",&xi,&ri,&gi,&di);
        double P = ri+gi;
        double X = xi;
        double tlo = X/vmax;
        double thi = X/vmin;
        long kstart = (long)floor((tlo - di - ri)/P) - 1;
        long kend = (long)floor((thi - di)/P) + 1;
        for(long k=kstart;k<=kend;k++){
            double a = di + (double)k*P;       // red start
            double b = di + ri + (double)k*P;  // red end
            if(b<=tlo || a>=thi) continue;
            double vl = X/b; // = xi/b
            double vr = X/a; // = xi/a
            if(vr<=vmin || vl>=vmax) continue;
            VL.push_back(vl);
            VR.push_back(vr);
            LID.push_back(i+1);
        }
    }
    
    // candidate points
    vector<double> cand;
    cand.reserve(VL.size()*2+2);
    cand.push_back(vmin);
    cand.push_back(vmax);
    for(size_t i=0;i<VL.size();i++){
        if(VL[i]>=vmin && VL[i]<=vmax) cand.push_back(VL[i]);
        if(VR[i]>=vmin && VR[i]<=vmax) cand.push_back(VR[i]);
    }
    
    // sorted lefts and rights for counting coverage
    vector<double> sortedL = VL;
    vector<double> sortedR = VR;
    sort(sortedL.begin(), sortedL.end());
    sort(sortedR.begin(), sortedR.end());
    
    // find min coverage, max p
    int bestCov = INT_MAX;
    double bestV = vmin;
    for(double p : cand){
        // count vl < p
        long less = lower_bound(sortedL.begin(), sortedL.end(), p) - sortedL.begin();
        // count vr <= p
        long leq = upper_bound(sortedR.begin(), sortedR.end(), p) - sortedR.begin();
        int cov = (int)(less - leq);
        if(cov < bestCov || (cov==bestCov && p > bestV)){
            bestCov = cov;
            bestV = p;
        }
    }
    
    // collect bad lights at bestV
    vector<char> bad(n+1,0);
    double p = bestV;
    int cnt=0;
    for(size_t i=0;i<VL.size();i++){
        if(VL[i] < p && p < VR[i]){
            int id=LID[i];
            if(!bad[id]){ bad[id]=1; cnt++; }
        }
    }
    
    printf("%.10f\n", bestV);
    printf("%d\n", cnt);
    bool first=true;
    for(int i=1;i<=n;i++){
        if(bad[i]){
            if(!first) printf(" ");
            printf("%d", i);
            first=false;
        }
    }
    printf("\n");
    
    return 0;
}
