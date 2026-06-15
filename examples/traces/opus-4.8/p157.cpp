#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; 
    if(!(cin>>n)) return 0;
    // Catalan number C_n = (1/(n+1)) * binom(2n,n)
    // compute binom(2n,n)
    unsigned long long res=1;
    for(int i=1;i<=n;i++){
        res = res*(unsigned long long)(n+i)/i;
    }
    res/= (unsigned long long)(n+1);
    cout<<res<<endl;
    return 0;
}
