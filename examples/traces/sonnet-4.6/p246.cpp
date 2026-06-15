#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int main(){
    string line;
    while(getline(cin,line)){
        ll val;
        if(sscanf(line.c_str(),"%lld",&val)==1){
            ll M=val;
            ll Np1=(M+3)/2;
            ll g=__gcd(Np1,(ll)3);
            ll max_nb=g*((M/g)/2);
            printf("%lld\n",max_nb+1);
        }
    }
}
