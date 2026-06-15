#include <bits/stdc++.h>
using namespace std;

int main(){
    int T, N;
    scanf("%d %d", &T, &N);
    while(T--){
        int M = 3*N;
        vector<long long> X(M);
        for(int i=0;i<M;i++) scanf("%lld", &X[i]);
        sort(X.begin(), X.end());
        long long ans = 0;
        for(int i=1;i<=N;i++){
            long long y1 = X[N + 2*i - 2];
            long long y2 = X[N + 2*i - 1];
            long long bp = X[N - i];
            ans += y1 * (y2 - bp);
        }
        printf("%lld\n", ans);
    }
    return 0;
}
