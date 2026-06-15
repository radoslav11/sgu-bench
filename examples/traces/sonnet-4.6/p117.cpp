#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,m,k;
    cin>>n>>m>>k;
    int count=0;
    for(int i=0;i<n;i++){
        long long x;
        cin>>x;
        // compute x^m mod k
        // but x^m can be huge, we just need x^m % k == 0
        // We can compute (x % k)^m % k
        long long val = 1;
        long long base = x % k;
        for(int j=0;j<m;j++){
            val = (val * base) % k;
        }
        if(val == 0) count++;
    }
    cout<<count<<endl;
    return 0;
}
