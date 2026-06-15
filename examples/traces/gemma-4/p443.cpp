#include <iostream>
#include <vector>

using namespace std;

/**
 * The problem asks to find the "key number" of two integers a and b.
 * The key number of a positive integer n is defined as the largest prime 
 * factor minus the total sum of all its other distinct prime factors.
 * We need to identify which door (a or b) has the larger key number.
 *
 * Constraints:
 * 2 <= a, b <= 10^6
 * Time Limit: 0.5s
 * Memory Limit: 262144 KB
 * 
 * Approach:
 * 1. For each integer n, find its distinct prime factors.
 * 2. Since n <= 10^6, we can perform prime factorization in O(sqrt(n)) time.
 * 3. Store the distinct prime factors in a sorted vector.
 * 4. Compute key_number = (largest prime factor) - (sum of all other prime factors).
 * 5. Compare key_number(a) and key_number(b) and output 'a' or 'b' accordingly.
 */

/**
 * Computes the key number of a given integer n.
 * n is at most 10^6, so prime factors can be found efficiently.
 * 
 * @param n The positive integer.
 * @return The calculated key number.
 */
long long get_key_number(int n) {
    vector<int> factors;
    int temp = n;
    
    // Iteratively find prime factors using trial division up to sqrt(temp)
    for (int d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            factors.push_back(d);
            // Ensure each prime factor is counted only once
            while (temp % d == 0) {
                temp /= d;
            }
        }
    }
    // If temp > 1, it means the remaining temp is a prime factor
    if (temp > 1) {
        factors.push_back(temp);
    }
    
    // Given 2 <= n, factors will never be empty.
    if (factors.empty()) return 0;

    // The largest prime factor is the last one in the sorted 'factors' vector.
    int max_p = factors.back();
    long long sum_others = 0;
    
    // Sum up all other distinct prime factors.
    for (size_t i = 0; i < factors.size() - 1; ++i) {
        sum_others += factors[i];
    }
    
    // Return the key number.
    return (long long)max_p - sum_others;
}

int main() {
    // Optimize I/O operations.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int a, b;
    // Read input integers a and b.
    if (cin >> a >> b) {
        // Calculate key numbers for a and b.
        long long key_a = get_key_number(a);
        long long key_b = get_key_number(b);
        
        // Compare key numbers and print 'a' if a's key number is larger, else print 'b'.
        // The problem guarantees that the key numbers are distinct.
        if (key_a > key_b) {
            cout << "a" << endl;
        } else {
            cout << "b" << endl;
        }
    }

    return 0;
}

