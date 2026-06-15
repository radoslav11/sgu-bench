#include <iostream>
using namespace std;

long long digit_sum(long long n) {
    long long s = 0;
    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int main() {
    long long l, r, k;
    cin >> l >> r >> k;
    long long count = 0;
    long long current = l;
    while (current <= r) {
        long long sum = 0;
        while (current <= r && sum < k) {
            sum += digit_sum(current);
            current++;
        }
        if (sum >= k) {
            count++;
        }
    }
    cout << count << endl;
    return 0;
}
