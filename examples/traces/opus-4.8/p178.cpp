#include <bits/stdc++.h>
using namespace std;
int main(){
    unsigned long long n;
    cin>>n;
    for(unsigned long long k=0;;k++){
        // max with k breaks: (k+1)*2^k
        unsigned long long cap;
        // careful overflow
        if(k>=60){cout<<k;return 0;}
        cap=(k+1)*(1ULL<<k);
        if(cap>=n){cout<<k;return 0;}
    }
}
