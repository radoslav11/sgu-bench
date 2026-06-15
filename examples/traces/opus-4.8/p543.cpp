#include <bits/stdc++.h>
using namespace std;
int main(){
    long long n,r; scanf("%lld %lld",&n,&r);
    long long S=0, waste=0;
    for(int i=0;i<n;i++){
        long long a; scanf("%lld",&a);
        S+=a;
        long long rem=a%r;
        if(rem==1) waste += (r-1);
    }
    long long T=(S+waste+r-1)/r;
    printf("%lld\n",T);
    return 0;
}
