#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

/**
 * Problem: Magic Pairs
 * The problem asks for all pairs (A, B) with 0 <= A, B < N such that 
 * for any integers X and Y, if A0X + B0Y is divisible by N, then AX + BY is divisible by N.
 * 
 * Let H be the subgroup of Z_N^2 defined by H = {(X, Y) in Z_N^2 | A0X + B0Y = 0 (mod N)}.
 * We want to find all (A, B) in Z_N^2 that satisfy AX + BY = 0 (mod N) for all (X, Y) in H.
 * This set of pairs (A, B) is the annihilator of H in Z_N^2.
 * 
 * The size of H can be calculated:
 * A0X + B0Y = 0 (mod N) is equivalent to (A0/g)X + (B0/g)Y = 0 (mod N/g), where g = gcd(A0, B0, N).
 * Let A' = A0/g, B' = B0/g, N' = N/g. Then gcd(A', B', N') = 1.
 * For a fixed Y, the number of X in {0, ..., N-1} such that A'X = -B'Y (mod N') is:
 *   Let d = gcd(A', N'). A'X = -B'Y (mod N') has solutions if and only if d | B'Y.
 *   Since gcd(A', B', N') = 1, we must have gcd(d, B') = 1. Thus d | B'Y iff d | Y.
 *   When solutions exist, there are d solutions in {0, ..., N'-1}, and thus d * (N/N') = d*g solutions in {0, ..., N-1}.
 *   The number of Y in {0, ..., N-1} divisible by d is N/d.
 *   Total size |H| = (N/d) * (d * g) = N * g.
 * 
 * The size of the annihilator of H is |Z_N^2| / |H| = N^2 / (N * g) = N / g.
 * The elements of the annihilator are of the form (c * A0 (mod N), c * B0 (mod N)) for c = 0, 1, ..., N/g - 1.
 */

using namespace std;

// Function to compute the greatest common divisor of two numbers
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N, A0, B0;
    // Read input
    if (!(cin >> N >> A0 >> B0)) return 0;

    // Normalize A0 and B0 to be within [0, N-1]
    A0 %= N;
    B0 %= N;

    // Calculate g = gcd(A0, B0, N)
    long long g = gcd(A0, gcd(B0, N));
    
    // N_prime is the number of unique pairs (A, B)
    long long N_prime = N / g;

    // Store all valid pairs (A, B)
    vector<pair<int, int>> solutions;
    solutions.reserve(N_prime);

    // Generate all solutions using the derived formula
    for (long long c = 0; c < N_prime; ++c) {
        int A = (int)((c * A0) % N);
        int B = (int)((c * B0) % N);
        solutions.push_back({A, B});
    }

    // Sort the pairs by A first, then by B
    sort(solutions.begin(), solutions.end());

    // Output the number of pairs
    cout << solutions.size() << "\n";

    // Output each pair
    for (const auto& p : solutions) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}

