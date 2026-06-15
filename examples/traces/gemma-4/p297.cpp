#include <iostream>

/**
 * Problem Analysis:
 * - N friends, M pubs.
 * - In each pub i, the total bill Si is divided among N friends as equally as possible.
 * - In pub i, Ri = Si % N friends pay (floor(Si / N) + 1) rubles, and the others pay floor(Si / N).
 * - The strategy is to distribute these extra rubles such that the total difference between
 *   any two friends' total payments is at most 1 ruble.
 * - Let Ti be the total amount friend i pays after M pubs.
 * - The total amount paid by all friends is T_total = sum(Si for i in 1 to M).
 * - Since the difference between any two total payments is at most 1, each friend will pay
 *   either floor(T_total / N) or floor(T_total / N) + 1.
 * - The number of friends X who pay one ruble more (floor(T_total / N) + 1) is T_total % N.
 *
 * Constraints:
 * - N, M <= 100
 * - Si <= 10,000
 * - Total sum <= 1,000,000 (fits in a 32-bit integer)
 * - Time limit: 0.25s (O(M) solution is efficient enough)
 */

using namespace std;

int main() {
    // Optimization for faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    // Read N and M from standard input.
    if (!(cin >> N >> M)) return 0;

    long long total_sum = 0;
    for (int i = 0; i < M; ++i) {
        int Si;
        // Read each pub's total bill Si.
        if (cin >> Si) {
            total_sum += Si;
        }
    }

    // The number of friends who pay 1 ruble more than the others is the remainder
    // of the total sum divided by the number of friends.
    // If N is 1, total_sum % 1 is always 0, which is correct.
    if (N > 0) {
        cout << (total_sum % N) << endl;
    } else {
        // According to constraints, 1 <= N, but handling N=0 just in case.
        cout << 0 << endl;
    }

    return 0;
}

