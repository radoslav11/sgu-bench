#include <iostream>
#include <algorithm>

using namespace std;

long long gcd(long long a, long long b) {
    a = abs(a);
    b = abs(b);
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int main() {
    int N;
    int caseNum = 1;
    while (cin >> N) {
        long long num, den;
        if (N == 1) {
            num = -1;
            den = 1;
        } else if (N <= 4) {
            num = 0;
            den = 1;
        } else {
            num = 4 - N;
            den = N - 2;
            long long common = gcd(num, den);
            num /= common;
            den /= common;
        }
        if (den < 0) {
            num = -num;
            den = -den;
        }
        cout << "Case #" << caseNum++ << ": " << num << "/" << den << endl;
    }
    return 0;
}

