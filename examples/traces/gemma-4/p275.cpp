#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * Problem: Find the maximum XOR sum of a subsequence.
 * 
 * Approach:
 * The problem can be solved using the concept of a Linear Basis. 
 * A linear basis of a set of numbers is a subset that can generate 
 * the same XOR space (all possible XOR sums) as the original set.
 * For 64-bit integers, the basis will have at most 64 elements.
 * 
 * Steps:
 * 1. Build the linear basis by iterating through each number in the input.
 *    For each number, attempt to insert it into the basis.
 * 2. Once the basis is built, find the maximum possible XOR sum by 
 *    iteratively checking each basis element from the most significant bit.
 * 
 * Time Complexity: O(N * log(max_Ai)), where N is the number of elements 
 * and log(max_Ai) is the maximum number of bits (approximately 60 for 10^18).
 * Given N <= 100 and log(max_Ai) <= 60, total operations are about 6000, 
 * which easily fits in the 0.25s time limit.
 * 
 * Space Complexity: O(log(max_Ai)) to store the basis elements.
 */

typedef long long ll;

// basis[i] will store a basis element whose highest set bit is at position i.
ll basis[64];

/**
 * Inserts a number x into the linear basis.
 * 
 * @param x The number to be inserted into the basis.
 */
void insert(ll x) {
    // Iterate from the most significant bit down to 0.
    // 10^18 is less than 2^60, so iterating from 62 is sufficient.
    for (int i = 62; i >= 0; --i) {
        // Check if the i-th bit of x is set.
        if (!(x & (1LL << i))) {
            continue;
        }
        
        // If basis[i] is empty, we can insert x here.
        if (!basis[i]) {
            basis[i] = x;
            return;
        }
        
        // Otherwise, reduce x using the current basis element and continue.
        x ^= basis[i];
    }
}

/**
 * Computes the maximum XOR sum possible by combining basis elements.
 * 
 * @return The maximum XOR sum.
 */
ll getMaxXor() {
    ll max_xor = 0;
    // Iterate from the most significant bit down to 0.
    for (int i = 62; i >= 0; --i) {
        // Greedily update max_xor: if XORing with basis[i] results in a 
        // larger value, update max_xor. Because basis[i]'s highest set 
        // bit is i, XORing it will only potentially change bit i and lower.
        if ((max_xor ^ basis[i]) > max_xor) {
            max_xor ^= basis[i];
        }
    }
    return max_xor;
}

int main() {
    // Optimize standard I/O for speed.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    // Read the number of elements in the sequence.
    if (!(cin >> N)) {
        return 0;
    }

    // Initialize the basis array with zeros.
    memset(basis, 0, sizeof(basis));

    // Read each number and insert it into the linear basis.
    for (int i = 0; i < N; ++i) {
        ll a;
        if (!(cin >> a)) {
            break;
        }
        insert(a);
    }

    // Output the maximum possible XOR sum.
    cout << getMaxXor() << endl;

    return 0;
}

