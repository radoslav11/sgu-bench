#include <bits/stdc++.h>
using namespace std;

int main(){
    long long L;
    if(!(cin>>L)) return 0;
    int M; cin>>M;
    vector<double> P(M);
    for(int i=0;i<M;i++) cin>>P[i];
    int N; cin>>N;
    vector<double> D(N);
    D[0]=0;
    for(int i=1;i<N;i++) cin>>D[i];
    double DN = D[N-1];
    double lo=0, hi=(double)L - DN;
    
    // gather candidates
    vector<double> cands;
    cands.push_back(lo);
    cands.push_back(hi);
    // xj = Pi - Dj
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            double s = P[i]-D[j];
            if(s>=lo-1e-9 && s<=hi+1e-9) cands.push_back(s);
        }
        // midpoints between adjacent door offsets (in S space)
        // for passenger i sorted xj descending as Dj ascending: xj = Pi-Dj
        for(int j=0;j+1<N;j++){
            double s = P[i] - (D[j]+D[j+1])/2.0;
            if(s>=lo-1e-9 && s<=hi+1e-9) cands.push_back(s);
        }
    }
    // clip
    for(double &c : cands){
        if(c<lo) c=lo;
        if(c>hi) c=hi;
    }
    sort(cands.begin(),cands.end());
    cands.erase(unique(cands.begin(),cands.end(),[](double a,double b){return fabs(a-b)<1e-9;}),cands.end());
    
    auto nearestDoor=[&](double t)->double{
        // distance from t to nearest D
        int idx = lower_bound(D.begin(),D.end(),t)-D.begin();
        double best=1e18;
        if(idx<N) best=min(best, fabs(D[idx]-t));
        if(idx-1>=0) best=min(best, fabs(D[idx-1]-t));
        return best;
    };
    
    double bestS=lo, bestSum=-1;
    for(double S : cands){
        double sum=0;
        for(int i=0;i<M;i++){
            double t = P[i]-S;
            sum += nearestDoor(t);
        }
        if(sum>bestSum){
            bestSum=sum;
            bestS=S;
        }
    }
    
    auto printHalf=[&](double v){
        long long twice = llround(v*2.0);
        if(twice%2==0){
            printf("%lld", twice/2);
        } else {
            long long ip = twice/2; // toward zero for positive
            // for nonnegative values
            printf("%lld.5", ip);
        }
    };
    
    printHalf(bestS);
    printf(" ");
    printHalf(bestSum);
    printf("\n");
    return 0;
}
