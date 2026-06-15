#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    vector<long long> a(n);
    long long F=0;
    for(int i=0;i<n;i++){scanf("%lld",&a[i]); F=max(F,a[i]);}
    long long total=0;
    for(int i=0;i<n;i++){
        long long c=a[i];
        long long flightsDone=0;
        while(true){
            long long r=F-flightsDone;
            long long A=r-c;
            if(A==0) break;
            if(c<=A){ total++; c*=2; }
            else { flightsDone += (c-A); c=A; total++; c*=2; }
        }
    }
    long long K=F+total;
    printf("%lld\n",K);
    if(K<=1000){
        int FF=(int)F;
        vector<vector<int>> gaps(FF);
        for(int i=0;i<n;i++){
            long long c=a[i];
            long long flightsDone=0;
            while(true){
                long long r=F-flightsDone;
                long long A=r-c;
                if(A==0) break;
                if(c<=A){ gaps[(int)flightsDone].push_back(i+1); c*=2; }
                else { flightsDone += (c-A); c=A; gaps[(int)flightsDone].push_back(i+1); c*=2; }
            }
        }
        for(int g=0; g<FF; g++){
            for(int pn: gaps[g]) printf("science mission to the planet %d\n", pn);
            printf("flying mission\n");
        }
    }
    return 0;
}
