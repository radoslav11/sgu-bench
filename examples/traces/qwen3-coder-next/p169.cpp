#include <iostream>
using namespace std;

int main() {
    long long K;
    cin >> K;

    int count = 0;
    for (int d = 1; d <= 8; d++) {
        long long modulus = 9LL * d * (d + 1);
        long long res = 1;
        long long base = 10 % modulus;
        long long exp = K;
        while (exp > 0) {
            if (exp & 1) {
                res = (res * base) % modulus;
            }
            base = (base * base) % modulus;
            exp >>= 1;
        }
        long long target = 10 % modulus;
        if (res == target) {
            count++;
        }
    }
    cout << count << endl;
    return 0;
}
