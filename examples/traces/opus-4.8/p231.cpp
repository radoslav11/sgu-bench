#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    scanf("%d",&n);
    vector<char> sieve(n+1,1);
    if(n>=0) sieve[0]=0;
    if(n>=1) sieve[1]=0;
    for(int i=2;(long long)i*i<=n;i++)
        if(sieve[i])
            for(int j=i*i;j<=n;j+=i) sieve[j]=0;
    vector<int> bs;
    for(int b=2;b+2<=n;b++){
        if(sieve[b] && sieve[b+2]) bs.push_back(b);
    }
    printf("%d\n",(int)bs.size());
    for(int b:bs) printf("2 %d\n",b);
    return 0;
}
