#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; long long M, Y;
    scanf("%d %lld %lld", &n, &M, &Y);
    vector<long long> X(n), K(n), rem(n);
    long long sum=0;
    for(int i=0;i<n;i++){
        scanf("%lld",&X[i]);
        long long prod = X[i]*M;
        K[i] = prod / Y;
        rem[i] = prod % Y;
        sum += K[i];
    }
    long long extra = M - sum;
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a,int b){
        return rem[a] > rem[b];
    });
    for(int i=0;i<extra;i++) K[idx[i]]++;
    for(int i=0;i<n;i++) printf("%lld ", K[i]);
    printf("\n");
    return 0;
}
