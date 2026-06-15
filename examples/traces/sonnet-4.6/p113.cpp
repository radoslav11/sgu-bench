#include<bits/stdc++.h>
using namespace std;

bool isPrime(long long n){
    if(n<2) return false;
    if(n==2) return true;
    if(n%2==0) return false;
    for(long long i=3;i*i<=n;i+=2){
        if(n%i==0) return false;
    }
    return true;
}

bool isNearlyPrime(long long n){
    // find p1 such that p1 divides n and p1 is prime
    // then check if n/p1 is prime
    for(long long i=2;i*i<=n;i++){
        if(n%i==0){
            if(isPrime(i) && isPrime(n/i)){
                return true;
            }
        }
    }
    return false;
}

int main(){
    int n;
    cin>>n;
    for(int i=0;i<n;i++){
        long long x;
        cin>>x;
        if(isNearlyPrime(x)) cout<<"Yes"<<endl;
        else cout<<"No"<<endl;
    }
    return 0;
}
