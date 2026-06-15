#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
    long long x, y;
    cin >> x >> y;

    // First, reduce the fraction to its simplest form
    long long g = __gcd(x, y);
    x /= g;
    y /= g;

    // Remove factors of 2 and 5 from denominator to determine the non-recurring part
    long long temp = y;
    int count2 = 0, count5 = 0;
    while (temp % 2 == 0) {
        count2++;
        temp /= 2;
    }
    while (temp % 5 == 0) {
        count5++;
        temp /= 5;
    }

    // The number of digits before recurring part starts is max(count2, count5)
    int nonRecurringLength = max(count2, count5);

    // If after removing factors of 2 and 5, the denominator becomes 1, 
    // then it's a finite decimal
    if (temp == 1) {
        cout << nonRecurringLength << " 0" << endl;
        return 0;
    }

    // Otherwise, find the length of the recurring part
    // The length of the recurring part is the multiplicative order of 10 modulo temp
    // That is, the smallest positive integer k such that 10^k ≡ 1 (mod temp)
    long long k = 1;
    long long remainder = 10 % temp;
    while (remainder != 1) {
        remainder = (remainder * 10) % temp;
        k++;
        // Safety check: k should be at most temp-1 by Euler's theorem
        if (k > temp) {
            // This shouldn't happen for valid inputs, but just in case
            break;
        }
    }

    cout << nonRecurringLength << " " << k << endl;

    return 0;
}
