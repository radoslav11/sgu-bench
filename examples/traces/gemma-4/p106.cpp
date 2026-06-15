#include <iostream>
#include <algorithm>

using namespace std;

/**
 * Problem: Find the number of integer solutions (x, y) for the equation ax + by + c = 0
 * such that x1 <= x <= x2 and y1 <= y <= y2.
 * 
 * The equation ax + by + c = 0 is equivalent to ax + by = -c.
 * This is a linear Diophantine equation.
 * 
 * Time limit: 0.25 sec, which is quite tight. The algorithm used is O(log(min(|a|, |b|))).
 */

typedef long long ll;

/**
 * Extended Euclidean Algorithm
 * Finds g = gcd(a, b) and integers x, y such that ax + by = g.
 */
ll gcd_ext(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = gcd_ext(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

/**
 * Integer floor division for positive divisor b.
 * Returns the largest integer k such that k <= a / b.
 */
ll floor_div(ll a, ll b) {
    if (a >= 0) return a / b;
    else return (a - b + 1) / b;
}

/**
 * Integer ceiling division for positive divisor b.
 * Returns the smallest integer k such that k >= a / b.
 */
ll ceil_div(ll a, ll b) {
    if (a >= 0) return (a + b - 1) / b;
    else return a / b;
}

struct Range {
    ll min_k, max_k;
};

/**
 * Given a constraint P + k*Q in [L, R], find the range of integers k.
 */
Range get_range(ll P, ll Q, ll L, ll R) {
    if (Q == 0) {
        // If Q is 0, P + 0*k = P is a constant.
        // If P is in the range [L, R], k can be any integer.
        if (P >= L && P <= R) return {-2000000000000000000LL, 2000000000000000000LL};
        else return {1, 0}; // Empty range
    }
    if (Q > 0) {
        // L <= P + kQ <= R  =>  L - P <= kQ <= R - P
        return {ceil_div(L - P, Q), floor_div(R - P, Q)};
    } else {
        // L <= P + kQ <= R  =>  L - P <= kQ <= R - P
        // Since Q < 0, let Q' = -Q > 0.
        // L - P <= -kQ' <= R - P  =>  P - R <= kQ' <= P - L
        ll Q_prime = -Q;
        return {ceil_div(P - R, Q_prime), floor_div(P - L, Q_prime)};
    }
}

int main() {
    // Standard I/O speedup
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll a, b, c, x1, x2, y1, y2;
    if (!(cin >> a >> b >> c >> x1 >> x2 >> y1 >> y2)) return 0;

    // If the interval is empty, there are no solutions.
    if (x1 > x2 || y1 > y2) {
        cout << 0 << endl;
        return 0;
    }

    // Handle cases where one or both coefficients are zero.
    if (a == 0 && b == 0) {
        if (c == 0) {
            // If 0x + 0y + 0 = 0, all (x, y) in the range are solutions.
            cout << (x2 - x1 + 1) * (y2 - y1 + 1) << endl;
        } else {
            cout << 0 << endl;
        }
        return 0;
    }

    if (a == 0) {
        // by + c = 0 => by = -c
        if ((-c) % b == 0) {
            ll y = (-c) / b;
            if (y >= y1 && y <= y2) cout << (x2 - x1 + 1) << endl;
            else cout << 0 << endl;
        } else {
            cout << 0 << endl;
        }
        return 0;
    }

    if (b == 0) {
        // ax + c = 0 => ax = -c
        if ((-c) % a == 0) {
            ll x = (-c) / a;
            if (x >= x1 && x <= x2) cout << (y2 - y1 + 1) << endl;
            else cout << 0 << endl;
        } else {
            cout << 0 << endl;
        }
        return 0;
    }

    // General case: ax + by = -c
    // Convert to A|x| + B|y| = -c where A, B are absolute values.
    ll A = (a < 0) ? -a : a;
    ll B = (b < 0) ? -b : b;
    ll C = -c;
    ll x_g, y_g;
    ll g = gcd_ext(A, B, x_g, y_g);

    // A linear Diophantine equation ax + by = C has solutions if and only if g | C.
    if (C % g != 0) {
        cout << 0 << endl;
        return 0;
    }

    // Particular solution (X0, Y0) for AX + BY = C.
    // Using __int128 to prevent potential overflow during calculation.
    __int128 X0 = (__int128)x_g * (C / g);
    __int128 Y0 = (__int128)y_g * (C / g);
    
    // Convert back to ax + by = -c using the sign of the original coefficients.
    // Let a = sa * A, b = sb * B, where sa, sb in {1, -1}.
    // Then sa*A*x + sb*B*y = -c <=> A(sa*x) + B(sb*y) = -c.
    // Let X = sa*x, Y = sb*y. X0 and Y0 satisfy AX0 + BY0 = C.
    // Thus x0 = X0/sa = X0*sa and y0 = Y0/sb = Y0*sb.
    ll sa = (a > 0) ? 1 : -1;
    ll sb = (b > 0) ? 1 : -1;
    
    ll x0 = (ll)(X0 * sa);
    ll y0 = (ll)(Y0 * sb);

    // The general solution for ax + by = -c is:
    // x = x0 + k * (b / g)
    // y = y0 - k * (a / g)
    ll dx = b / g;
    ll dy = -a / g;

    // Determine the valid range for k derived from x and y constraints.
    Range rx = get_range(x0, dx, x1, x2);
    Range ry = get_range(y0, dy, y1, y2);

    // The final k range is the intersection of rx and ry.
    ll k_min = max(rx.min_k, ry.min_k);
    ll k_max = min(rx.max_k, ry.max_k);

    if (k_min <= k_max) {
        cout << (k_max - k_min + 1) << endl;
    } else {
        cout << 0 << endl;
    }

    return 0;
}

