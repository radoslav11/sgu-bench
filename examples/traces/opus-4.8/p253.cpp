#include <bits/stdc++.h>
using namespace std;

int N, M, K;
vector<long long> X, Y;

inline long long cross(long long ox, long long oy, long long ax, long long ay, long long bx, long long by){
    return (ax-ox)*(by-oy) - (ay-oy)*(bx-ox);
}

bool inside(long long px, long long py){
    long long c1 = cross(X[0],Y[0],X[1],Y[1],px,py);
    long long cn = cross(X[0],Y[0],X[N-1],Y[N-1],px,py);
    if(c1 < 0 || cn > 0) return false;
    int lo = 1, hi = N-1;
    while(hi - lo > 1){
        int mid = (lo+hi)/2;
        if(cross(X[0],Y[0],X[mid],Y[mid],px,py) >= 0) lo = mid;
        else hi = mid;
    }
    long long c = cross(X[lo],Y[lo],X[lo+1],Y[lo+1],px,py);
    return c >= 0;
}

int main(){
    scanf("%d %d %d", &N, &M, &K);
    X.resize(N); Y.resize(N);
    for(int i=0;i<N;i++) scanf("%lld %lld", &X[i], &Y[i]);
    int cnt = 0;
    for(int i=0;i<M;i++){
        long long px, py;
        scanf("%lld %lld", &px, &py);
        if(inside(px,py)) cnt++;
    }
    printf(cnt>=K ? "YES\n" : "NO\n");
    return 0;
}
