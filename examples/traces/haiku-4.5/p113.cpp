#include <iostream>
using namespace std;

bool isPrime(long long n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

long long getSmallestPrimeFactor(long long n) {
    if (n % 2 == 0) return 2;
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return i;
    }
    return n;
}

bool isNearlyPrime(long long n) {
    if (n < 4) return false;
    
    long long p1 = getSmallestPrimeFactor(n);
    long long p2 = n / p1;
    
    return isPrime(p2);
}

int main() {
    int N;
    cin >> N;
    
    for (int i = 0; i < N; i++) {
        long long num;
        cin >> num;
        
        if (isNearlyPrime(num)) {
            cout << "Yes\n";
        } else {
            cout << "No\n";
        }
    }
    
    return 0;
}
