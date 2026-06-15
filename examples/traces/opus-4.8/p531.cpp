#include <bits/stdc++.h>
using namespace std;

int main(){
    int n; long long d;
    scanf("%d %lld", &n, &d);
    vector<long long> x(n), w(n);
    for(int i=0;i<n;i++) scanf("%lld %lld", &x[i], &w[i]);
    
    // prefix max of w with index
    vector<long long> prefMax(n);
    vector<int> prefIdx(n);
    for(int i=0;i<n;i++){
        if(i==0 || w[i] > prefMax[i-1]){
            prefMax[i] = w[i];
            prefIdx[i] = i;
        } else {
            prefMax[i] = prefMax[i-1];
            prefIdx[i] = prefIdx[i-1];
        }
    }
    
    long long best = -1;
    int bi=-1, bj=-1;
    for(int i=0;i<n;i++){
        // find rightmost j with x[j] <= x[i]-d
        long long target = x[i]-d;
        // binary search
        int lo=0, hi=i-1, res=-1;
        // actually search in all, but j<i since x sorted and target < x[i]
        // upper_bound for target
        int l=0, r=n-1, pos=-1;
        while(l<=r){
            int m=(l+r)/2;
            if(x[m] <= target){ pos=m; l=m+1;}
            else r=m-1;
        }
        if(pos>=0){
            long long sum = w[i] + prefMax[pos];
            if(sum > best){
                best = sum;
                bi = i;
                bj = prefIdx[pos];
            }
        }
    }
    
    if(best<0) printf("-1 -1\n");
    else printf("%d %d\n", bi+1, bj+1);
    return 0;
}
