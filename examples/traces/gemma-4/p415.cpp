#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * We are given n coins with distinct denominations a_1, a_2, ..., a_n.
 * We need to find which coins are "absolutely necessary" to form a sum x.
 * A coin a_i is absolutely necessary if it is part of EVERY subset of coins
 * that sums up to x.
 * 
 * Logic:
 * A coin a_i is absolutely necessary if and only if it is impossible to
 * form the sum x using only the other n-1 coins (the set of all coins except a_i).
 * 
 * Dynamic Programming approach:
 * 1. Let pre[i] be a bitset where the j-th bit is 1 if the sum j can be formed 
 *    using a subset of the first i coins (a_1, ..., a_i).
 * 2. Let suf[i] be a bitset where the j-th bit is 1 if the sum j can be formed 
 *    using a subset of the coins from index i to n (a_i, ..., a_n).
 * 3. To check if a_i is necessary, we check if there's any sum j such that
 *    pre[i-1][j] is true AND suf[i+1][x-j] is true. If such a j exists, it means
 *    we can form the sum x without using coin a_i.
 * 
 * Complexity:
 * - Building pre and suf: O(n * x / W), where W is the word size (e.g., 64).
 * - Checking each coin: O(n * x).
 * - n = 200, x = 10,000. Total operations ~ 2 * 10^6, well within 1.25s limit.
 * - Memory: O(n * x) bits, about 0.5 MB, well within 256 MB limit.
 */

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, x;
    if (!(cin >> n >> x)) return 0;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    // pre[i] stores achievable sums using coins 1 to i
    // suf[i] stores achievable sums using coins i to n
    // Using 10001 to accommodate x up to 10,000
    vector<bitset<10001>> pre(n + 2);
    vector<bitset<10001>> suf(n + 2);

    // Base cases for DP
    pre[0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        pre[i] = pre[i - 1] | (pre[i - 1] << a[i]);
    }

    suf[n + 1][0] = 1;
    for (int i = n; i >= 1; --i) {
        suf[i] = suf[i + 1] | (suf[i + 1] << a[i]);
    }

    vector<int> necessary;
    for (int i = 1; i <= n; ++i) {
        bool possible_without_ai = false;
        
        // Check if x can be formed using coins {a_1, ..., a_{i-1}} and {a_{i+1}, ..., a_n}
        // This is done by checking if there is any j such that pre[i-1][j] and suf[i+1][x-j] are both true.
        // Since x <= 10,000, a simple loop is efficient.
        for (int j = 0; j <= x; ++j) {
            if (pre[i - 1][j] && suf[i + 1][x - j]) {
                possible_without_ai = true;
                break;
            }
        }

        // If it's impossible to form x without a_i, then a_i is necessary.
        if (!possible_without_ai) {
            necessary.push_back(a[i]);
        }
    }

    // Output the result
    cout << necessary.size() << "\n";
    for (int i = 0; i < (int)necessary.size(); ++i) {
        cout << necessary[i] << (i == (int)necessary.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

