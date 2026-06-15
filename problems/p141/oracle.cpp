#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

// Extended Euclidean algorithm.
// Returns gcd(a,b) and finds x,y such that a*x + b*y = gcd(a,b).
int64 ext_gcd(int64 a, int64 b, int64 &x, int64 &y) {
    if (b == 0) {
        x = 1;      // 1·a + 0·b = a
        y = 0;
        return a;
    }
    int64 x1, y1;
    int64 g = ext_gcd(b, a % b, x1, y1);
    // Back-substitute: b·x1 + (a%b)·y1 = g
    // (a%b) = a - (a/b)*b
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64 x1, x2, P, K;
    cin >> x1 >> x2 >> P >> K;

    // Step A: find one solution (a0,b0) to a·x1 + b·x2 = P
    int64 a0, b0;
    int64 g = ext_gcd(x1, x2, a0, b0);
    if (P % g != 0) {
        cout << "NO\n";
        return 0;
    }
    // Scale the particular solution by P/g
    a0 *= P / g;
    b0 *= P / g;

    // dx, dy are the shifts in a,b when we change the free parameter t by +1
    int64 dx = x2 / g;
    int64 dy = x1 / g;

    // Step C: optimize t to minimize |a|+|b|
    // We do a local descent: check if moving t by +1 or -1 reduces S.
    auto S = [&](int64 a, int64 b) {
        return llabs(a) + llabs(b);
    };

    int64 a = a0, b = b0;
    // Try shifting t upwards as long as it helps
    while (true) {
        int64 a_up = a + dx, b_up = b - dy;
        if (S(a_up, b_up) < S(a, b)) {
            a = a_up; b = b_up;
        } else break;
    }
    // Try shifting t downwards as long as it helps
    while (true) {
        int64 a_dn = a - dx, b_dn = b + dy;
        if (S(a_dn, b_dn) < S(a, b)) {
            a = a_dn; b = b_dn;
        } else break;
    }

    int64 sumAbs = S(a, b);
    if (sumAbs > K) {
        cout << "NO\n";
        return 0;
    }

    // rem = leftover jumps after using |a|+|b|
    int64 rem = K - sumAbs;

    // If rem is odd, we need to flip parity of sumAbs by shifting t by ±1
    if (rem % 2 != 0) {
        // Can we flip parity? Only if dx+dy is odd
        if ((dx + dy) % 2 == 0) {
            cout << "NO\n";
            return 0;
        }
        // Choose the direction that keeps S <= K
        int64 a_up = a + dx, b_up = b - dy;
        if (S(a_up, b_up) <= K) {
            a = a_up; b = b_up;
        } else {
            a = a - dx;
            b = b + dy;
        }
        // Recompute sumAbs and rem
        sumAbs = S(a, b);
        rem = K - sumAbs;
        // If it still fails, no solution
        if (rem < 0 || rem % 2 != 0) {
            cout << "NO\n";
            return 0;
        }
    }

    // Now rem is even. We can distribute rem/2 extra +x1 and rem/2 extra -x1.
    int64 P1, N1, P2, N2;
    // Base decomposition of a into P1,N1
    if (a >= 0) { P1 = a; N1 = 0; }
    else         { P1 = 0; N1 = -a; }
    // Base decomposition of b into P2,N2
    if (b >= 0) { P2 = b; N2 = 0; }
    else         { P2 = 0; N2 = -b; }

    // Insert cancelling jumps on x1: half +x1, half -x1
    int64 add = rem / 2;
    P1 += add;
    N1 += add;

    // We used |a| + |b| + 2*(rem/2) = K jumps total
    cout << "YES\n";
    cout << P1 << " " << N1 << " " << P2 << " " << N2 << "\n";
    return 0;
}
