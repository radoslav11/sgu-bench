#include <iostream>

/**
 * Problem: Boxes
 * The problem asks for the minimum number of moves to move all balls from two boxes
 * into a single box. A move consists of moving balls from one box to another,
 * where the number of balls moved equals the number of balls already in the 
 * receiving box.
 * 
 * Let a and b be the number of balls in the two boxes. Let S = a + b.
 * A move consists of:
 * - If a < b: move a balls from b to a, so the new state is (2a, b-a).
 * - If a > b: move b balls from a to b, so the new state is (a-b, 2b).
 * - If a = b: move a balls from one to the other, so the new state is (2a, 0) or (0, 2a).
 * 
 * In all cases, the new number of balls in the first box is (2a) mod S.
 * The goal is to reach a state where all balls are in one box, which means
 * (2^n * a) mod S = 0 for some n.
 * 
 * Let S = 2^p * m, where m is an odd number.
 * The condition (2^n * a) mod S = 0 is equivalent to:
 * 2^n * a is a multiple of 2^p * m.
 * Since m is odd, this means m must divide a.
 * If m divides a, let a = m * c.
 * Then 2^n * m * c = k * 2^p * m => 2^n * c = k * 2^p.
 * This holds when n + (number of times 2 divides c) >= p.
 * The minimum n is max(0, p - j), where j is the number of times 2 divides c.
 */

using namespace std;

int main() {
    // Speed up I/O operations.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long A, B;
    // Read A and B. S can be up to 2^31 - 1, so long long is used to avoid overflow.
    if (!(cin >> A >> B)) return 0;

    // If one box is already empty, no moves are needed.
    if (A == 0 || B == 0) {
        cout << 0 << endl;
        return 0;
    }

    long long S = A + B;
    long long m = S;
    int p = 0;

    // Find the odd part m of S and the power of 2 component p.
    while (m > 0 && m % 2 == 0) {
        m /= 2;
        p++;
    }

    // If m does not divide A, it's impossible to move all balls into one box.
    if (A % m != 0) {
        cout << -1 << endl;
    } else {
        // Let c = A / m. We need to find how many times 2 divides c.
        long long c = A / m;
        int j = 0;
        while (c > 0 && c % 2 == 0) {
            c /= 2;
            j++;
        }
        // The minimum number of moves is n = max(0, p - j).
        int n = p - j;
        if (n < 0) n = 0;
        cout << n << endl;
    }

    return 0;
}

