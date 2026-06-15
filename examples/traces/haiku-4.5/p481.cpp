#include<bits/stdc++.h>
using namespace std;

long long power(long long base, long long exp) {
    long long result = 1;
    while(exp > 0) {
        if(exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

int main(){
    int n;
    cin >> n;
    
    // Number of Mescher trees = n^(n-2) * (n-1)(n-2)/2
    long long trees = power(n, n-2);
    long long edges_to_add = (long long)(n-1) * (n-2) / 2;
    
    long long result = trees * edges_to_add;
    
    cout << result << endl;
    
    return 0;
}
