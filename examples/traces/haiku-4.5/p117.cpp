#include <iostream>
#include <cmath>
using namespace std;

long long modpow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main() {
    int N, M, K;
    cin >> N >> M >> K;
    
    int count = 0;
    for (int i = 0; i < N; i++) {
        int num;
        cin >> num;
        
        long long remainder = modpow(num, M, K);
        if (remainder == 0) {
            count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
