#include<bits/stdc++.h>
using namespace std;

long long getKeyNumber(int n) {
    set<int> primes;
    
    // Find all prime factors
    int temp = n;
    
    // Check for 2
    if (temp % 2 == 0) {
        primes.insert(2);
        while (temp % 2 == 0) {
            temp /= 2;
        }
    }
    
    // Check for odd factors from 3 onwards
    for (int i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            primes.insert(i);
            while (temp % i == 0) {
                temp /= i;
            }
        }
    }
    
    // If temp > 1, then it's a prime factor
    if (temp > 1) {
        primes.insert(temp);
    }
    
    // Calculate key number
    long long maxPrime = *primes.rbegin();
    long long sumOthers = 0;
    
    for (int p : primes) {
        if (p != maxPrime) {
            sumOthers += p;
        }
    }
    
    return maxPrime - sumOthers;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int a, b;
    cin >> a >> b;
    
    long long keyA = getKeyNumber(a);
    long long keyB = getKeyNumber(b);
    
    if (keyA > keyB) {
        cout << "a" << endl;
    } else {
        cout << "b" << endl;
    }
    
    return 0;
}
