#include <bits/stdc++.h>
using namespace std;
int main(){
    double L; int N;
    scanf("%lf %d",&L,&N);
    long long LL=llround(L*10000);
    long long pos=0;
    for(int i=0;i<N;i++){
        long long T,V;
        scanf("%lld %lld",&T,&V);
        long long tv=T*V;
        long long scaled=((tv%LL)*10000)%LL;
        pos=(pos+scaled)%LL;
    }
    double position=pos/10000.0;
    double dist=min(position,L-position);
    printf("%.4f\n",dist);
}
