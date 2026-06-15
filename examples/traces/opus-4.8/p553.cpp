#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<long long> w(n+1), c(n+1);
    for(int i=1;i<=n;i++){
        scanf("%lld %lld", &w[i], &c[i]);
    }
    vector<long long> Wp(n+1,0), Cp(n+1,0);
    for(int i=1;i<=n;i++){
        Wp[i]=Wp[i-1]+w[i];
        Cp[i]=Cp[i-1]+c[i];
    }
    long long Ctot = Cp[n];
    
    // H_max
    int hmax=0;
    int blim = n-m-1; // max b for table nonempty
    for(int b=1; b<=blim; b++){
        int base = n-b-m;
        long long Wh = Wp[n-b]-Wp[base];
        long long Wt = Wp[base]; // a=0
        if(Wh <= (long long)k*Wt){
            hmax=b;
        } else break;
    }
    
    long long bestVal=0;
    int bestH=0, bestT=0;
    
    for(int h=0; h<=hmax; h++){
        int base = n-h-m; // table top index, base>=1
        long long Wh = Wp[n-h]-Wp[base];
        // need k*Wp[a] <= k*Wp[base] - Wh, a in [0, base-1]
        long long Lim = (long long)k*Wp[base] - Wh;
        int cap = base-1; // = n-m-1-h
        // binary search largest a with k*Wp[a] <= Lim, a in [0,cap]
        int lo=0, hi=cap, best=0;
        // a=0 always valid since Lim>=0 (h<=hmax)
        while(lo<=hi){
            int mid=(lo+hi)/2;
            if((long long)k*Wp[mid] <= Lim){
                best=mid; lo=mid+1;
            } else hi=mid-1;
        }
        int t = best;
        long long val = (Ctot - Cp[n-h]) + Cp[t]; // suffix c for h + prefix c for t
        if(val > bestVal){
            bestVal=val; bestH=h; bestT=t;
        }
    }
    
    if(bestVal==0){
        printf("0 0\n");
    } else {
        int p = bestH + bestT;
        printf("%d %lld\n", p, bestVal);
        string s;
        for(int i=0;i<bestH;i++) s+='H';
        for(int i=0;i<bestT;i++) s+='T';
        printf("%s\n", s.c_str());
    }
    return 0;
}
