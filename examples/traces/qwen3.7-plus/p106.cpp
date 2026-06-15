#include <iostream>
#include <numeric>
#include <algorithm>

using namespace std;

long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long d = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

long long floor_div(long long P, long long Q) {
    if (P % Q == 0) return P / Q;
    return P / Q - (P < 0 ? 1 : 0);
}

long long ceil_div(long long P, long long Q) {
    if (P % Q == 0) return P / Q;
    return P / Q + (P > 0 ? 1 : 0);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long a, b, c, x1, x2, y1, y2;
    if (!(cin >> a >> b >> c >> x1 >> x2 >> y1 >> y2)) return 0;

    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);

    if (a == 0 && b == 0) {
        if (c == 0) {
            long long ans = (x2 - x1 + 1) * (y2 - y1 + 1);
            cout << ans << "\n";
        } else {
            cout << 0 << "\n";
        }
        return 0;
    }

    if (a == 0) {
        if (c % b == 0) {
            long long y = -c / b;
            if (y >= y1 && y <= y2) {
                cout << (x2 - x1 + 1) << "\n";
            } else {
                cout << 0 << "\n";
            }
        } else {
            cout << 0 << "\n";
        }
        return 0;
    }

    if (b == 0) {
        if (c % a == 0) {
            long long x = -c / a;
            if (x >= x1 && x <= x2) {
                cout << (y2 - y1 + 1) << "\n";
            } else {
                cout << 0 << "\n";
            }
        } else {
            cout << 0 << "\n";
        }
        return 0;
    }

    long long g = std::gcd(std::abs(a), std::abs(b));
    if (c % g != 0) {
        cout << 0 << "\n";
        return 0;
    }

    long long a_prime = a / g;
    long long b_prime = b / g;
    long long c_prime = -c / g;

    long long x0, y0;
    ext_gcd(a_prime, b_prime, x0, y0);
    
    // Ensure a_prime * x0 + b_prime * y0 == 1
    if (x0 * a_prime + y0 * b_prime != 1) {
        x0 = -x0;
        y0 = -y0;
    }

    x0 *= c_prime;
    y0 *= c_prime;

    long long low_x, high_x;
    long long b_abs = std::abs(b_prime);
    if (b_prime > 0) {
        low_x = ceil_div(x1 - x0, b_abs);
        high_x = floor_div(x2 - x0, b_abs);
    } else {
        low_x = ceil_div(x0 - x2, b_abs);
        high_x = floor_div(x0 - x1, b_abs);
    }

    long long low_y, high_y;
    long long a_abs = std::abs(a_prime);
    if (a_prime > 0) {
        low_y = ceil_div(y0 - y2, a_abs);
        high_y = floor_div(y0 - y1, a_abs);
    } else {
        low_y = ceil_div(y1 - y0, a_abs);
        high_y = floor_div(y2 - y0, a_abs);
    }

    long long k_low = max(low_x, low_y);
    long long k_high = min(high_x, high_y);

    long long ans = 0;
    if (k_low <= k_high) {
        ans = k_high - k_low + 1;
    }
    
    cout << ans << "\n";

    return 0;
}
