#include<bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    while(b) { long long t=b; b=a%b; a=t; }
    return a;
}

int main(){
    long long N, M;
    cin >> N >> M;
    
    long long count = 0;
    for(long long i = 1; i < N; i++) {
        for(long long j = 1; j < M; j++) {
            if(gcd(i, j) == 1) count++;
        }
    }
    count += (M - 1) + (N - 1);
    
    cout << count << endl;
    return 0;
}
