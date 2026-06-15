#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * We have n banks along a street, each with a position x_i and money w_i.
 * We need to find two banks such that the distance between them is at least d
 * and the sum of money in them is maximized.
 * 
 * Solution Strategy:
 * The banks are given in increasing order of position x_i.
 * For each bank j (considered as the second bank in a robbery pair), 
 * we want to find a bank i (the first bank) such that x_j - x_i >= d, 
 * and w_i is maximum.
 * 
 * Since banks are sorted by position, as we increment j, the set of banks i
 * that satisfy the distance condition (x_i <= x_j - d) can only grow.
 * We can maintain a sliding window or a pointer (left_ptr) to keep track 
 * of the valid banks i, and a variable (max_w_idx) to store the index of
 * the bank with the maximum money among all valid banks i found so far.
 * 
 * Time Complexity: O(n) - Each bank index is processed at most twice (once by j and once by left_ptr).
 * Space Complexity: O(n) - Storing bank information.
 */

struct Bank {
    int x, w, id;
};

int main() {
    // Speed up the standard input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long d;
    // Read n and the minimum distance d
    if (!(cin >> n >> d)) return 0;

    // Read the bank details
    vector<Bank> banks(n);
    for (int i = 0; i < n; ++i) {
        cin >> banks[i].x >> banks[i].w;
        banks[i].id = i + 1; // The banks are 1-indexed
    }

    // Variables to store the best result found
    long long max_sum = -1;
    int ans_i = -1, ans_j = -1;

    // left_ptr tracks the last bank index that satisfies the distance condition
    int left_ptr = -1;
    // max_w_idx tracks the bank index with the maximum money among [0, left_ptr]
    int max_w_idx = -1;

    // Iterate through each bank, treating it as the rightmost bank in the pair
    for (int j = 0; j < n; ++j) {
        // While the next bank's distance from current bank j is at least d,
        // include it in the candidate pool for the first bank.
        while (left_ptr + 1 < j && (long long)banks[j].x - banks[left_ptr + 1].x >= d) {
            left_ptr++;
            // Update the index of the bank with maximum money found so far
            if (max_w_idx == -1 || banks[left_ptr].w > banks[max_w_idx].w) {
                max_w_idx = left_ptr;
            }
        }

        // If we have found at least one bank satisfying the distance condition
        if (max_w_idx != -1) {
            long long current_sum = (long long)banks[max_w_idx].w + banks[j].w;
            // Update the overall maximum sum and the corresponding bank indices
            if (current_sum > max_sum) {
                max_sum = current_sum;
                ans_i = banks[max_w_idx].id;
                ans_j = banks[j].id;
            }
        }
    }

    // Output the result
    if (ans_i == -1) {
        // No such pair of banks found
        cout << "-1 -1" << endl;
    } else {
        // Print the 1-based indices of the two banks
        cout << ans_i << " " << ans_j << endl;
    }

    return 0;
}

