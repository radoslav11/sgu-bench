#include <iostream>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * The cyclist's position x1(T) and the pedestrian's position x2(T) can be analyzed
 * using a circle of circumference 2l. 
 * The cyclist's position on the circle is c1(T) = (v1 * T) mod 2l.
 * The pedestrian's position on the circle is c2(T) = (v2 * T + l) mod 2l.
 * 
 * A meeting occurs when x1(T) = x2(T). 
 * This happens when:
 * 1) c1(T) = c2(T)  => (v1 - v2) * T = (2k + 1) * l
 * 2) c1(T) = 2l - c2(T) => c1(T) + c2(T) = 2l (or multiple) => (v1 + v2) * T = (2m + 1) * l
 * 
 * We need to find the number of distinct meeting times T in [0, t].
 * Using the Principle of Inclusion-Exclusion:
 * Count = Count(Cond 1) + Count(Cond 2) - Count(Cond 1 AND Cond 2).
 * 
 * Cond 1 and Cond 2 are both true if c1(T) = c2(T) and c1(T) + c2(T) is a multiple of 2l.
 * This implies 2*c1(T) is a multiple of 2l, so c1(T) is 0 or l (mod 2l).
 * This only happens at the cities A or B.
 * 
 * This occurs when v1*T = (k' * l) and v2*T = (m' * l) where the specific combinations 
 * result in meeting at x=0 or x=l.
 * Specifically, the meeting time must satisfy:
 * (v1 - v2) * T = (2k + 1) * l
 * (v1 + v2) * T = (2m + 1) * l
 * 
 * Let V1 = |v1 - v2| and V2 = v1 + v2.
 * Let g = gcd(V1, V2). Let A = V1/g and B = V2/g.
 * For a simultaneous solution, T must satisfy (2k + 1)/A = (2m + 1)/B.
 * Since gcd(A, B) = 1, this implies 2k + 1 = n * A and 2m + 1 = n * B for some integer n.
 * For 2k + 1 and 2m + 1 to be odd, n must be odd AND A and B must be odd.
 * If A and B are both odd, the number of such T <= t is the count of odd n such that
 * (n * A * l) / V1 <= t  =>  (n * A * l) / (g * A) <= t  =>  n * l / g <= t  =>  n <= (g * t) / l.
 */

typedef long long ll;

// Using manual gcd to ensure compatibility
ll get_gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll l, v1, v2, t;
    if (!(cin >> l >> v1 >> v2 >> t)) return 0;

    ll V1 = abs(v1 - v2);
    ll V2 = v1 + v2;

    // Count meeting times from condition 1: (v1 - v2) * T = (2k + 1) * l
    ll count1 = 0;
    if (V1 > 0) {
        ll M1 = (V1 * t) / l;
        // Number of odd integers in [1, M1]
        count1 = (M1 + 1) / 2;
    }

    // Count meeting times from condition 2: (v1 + v2) * T = (2m + 1) * l
    ll count2 = 0;
    ll M2 = (V2 * t) / l;
    // Number of odd integers in [1, M2]
    count2 = (M2 + 1) / 2;

    // Count meeting times that satisfy both conditions
    ll count3 = 0;
    if (V1 > 0) {
        ll g = get_gcd(V1, V2);
        ll A = V1 / g;
        ll B = V2 / g;
        // Both A and B must be odd for there to be simultaneous odd-multiple solutions
        if (A % 2 != 0 && B % 2 != 0) {
            ll M3 = (g * t) / l;
            // Number of odd n in [1, M3]
            count3 = (M3 + 1) / 2;
        }
    }

    // The answer is the union of the two sets of meeting times
    cout << count1 + count2 - count3 << endl;

    return 0;
}

