#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Game for Little Johnny
 * Given N, find the number of pairs (A, B) such that 1 <= A < B <= N 
 * and there exist integers x, y >= 1 such that Ax + By = N.
 * 
 * Analysis:
 * Ax + By = N with x, y >= 1 is equivalent to:
 * There exists an integer y >= 1 such that By = N - Ax and x >= 1.
 * This means B must be a divisor of N - Ax for some x >= 1, and B > A.
 * The condition x >= 1 implies Ax <= N - B, or Ax <= N - (A + 1), so Ax <= N - A - 1.
 * The condition y >= 1 implies By <= N - A.
 * Since B is a divisor of N - Ax, B <= N - Ax.
 * 
 * Given the time limit and problem type, N is likely around 10^6.
 * We can iterate through A from 1 to N/2. For each A, we iterate through 
 * all possible values of V = N - Ax where x >= 1 and V >= A + 1.
 * For each such V, we look at its divisors B such that B > A.
 * We use a "last_seen" array to ensure each (A, B) pair is counted only once.
 * Total complexity is approximately O(N log^2 N).
 */

const int MAXN = 1000005;
vector<int> divisors[MAXN];
int last_seen[MAXN];
int count_divs[MAXN];

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    if (N < 3) {
        cout << 0 << endl;
        return 0;
    }

    // Precompute the number of divisors for each integer up to N
    // to reserve memory for the vectors, avoiding reallocations.
    for (int i = 1; i <= N; ++i) {
        for (int j = i; j <= N; j += i) {
            count_divs[j]++;
        }
    }
    for (int i = 1; i <= N; ++i) {
        divisors[i].reserve(count_divs[i]);
    }
    // Precompute all divisors up to N
    for (int i = 1; i <= N; ++i) {
        for (int j = i; j <= N; j += i) {
            divisors[j].push_back(i);
        }
    }

    long long ans = 0;
    // Iterate over all possible values of A
    for (int A = 1; A <= N / 2; ++A) {
        // Iterate over all x >= 1 such that V = N - Ax is at least A + 1
        for (int k = 1; ; ++k) {
            int V = N - A * k;
            // If V <= A, then no B > A can be a divisor of V
            if (V <= A) break;
            
            // Find the first divisor of V that is strictly greater than A.
            // Since divisors[V] is sorted, we can use upper_bound.
            auto it = upper_bound(divisors[V].begin(), divisors[V].end(), A);
            for (; it != divisors[V].end(); ++it) {
                int B = *it;
                // Check if this pair (A, B) has been counted for the current A.
                if (last_seen[B] != A) {
                    last_seen[B] = A;
                    ans++;
                }
            }
        }
    }

    cout << ans << endl;

    return 0;
}

