#include <bits/stdc++.h>
using namespace std;

int main(){
    int N; double d, c;
    cin >> N >> d >> c;
    vector<double> a(N), b(N), del(N);
    for(int i=0;i<N;i++) cin >> a[i] >> b[i] >> del[i];
    
    // distance on lane i from time t1 to t2
    auto dist=[&](int i,double t1,double t2){
        return b[i]*(t2-t1) - a[i]*(cos(t2+del[i])-cos(t1+del[i]));
    };
    
    // Stay on lane 1, find time to reach distance d
    int lane=0;
    double t=0, traveled=0;
    // binary search time
    double lo=0, hi=1e9;
    for(int it=0;it<200;it++){
        double mid=(lo+hi)/2;
        if(dist(lane,0,mid)>=d) hi=mid; else lo=mid;
    }
    
    printf("%.15f\n", hi);
    printf("0\n");
    return 0;
}
