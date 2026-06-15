#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

/**
 * Problem Analysis:
 * - The telephone directory has 2 initial pages (name and instructions).
 * - Phone number records start on the third page.
 * - Each page can contain at most K lines.
 * - Phone numbers must be sorted in increasing order.
 * - If the first digit of a phone number changes, that record MUST be on a new page.
 * - We need to find the total number of pages P in the directory.
 *
 * Strategy:
 * 1. Read K (max lines per page) and N (number of phone numbers).
 * 2. Read all N phone numbers. Since they are 4 digits, sorting them lexicographically 
 *    as strings is equivalent to sorting them numerically.
 * 3. Sort the phone numbers in ascending order.
 * 4. Group phone numbers that have the same first digit.
 * 5. For each group of size S, the number of pages needed for that group is ceil(S/K).
 *    Using integer division, this is (S + K - 1) / K.
 * 6. Sum the pages required for all groups and add 2 (the initial pages).
 * 7. Output the final result.
 *
 * Complexity:
 * - Sorting: O(N log N), where N < 8000.
 * - Iteration: O(N) to group and count pages.
 * - Space: O(N) to store the phone numbers.
 * - This is well within the 0.25s and 4MB time/memory limits.
 */

using namespace std;

int main() {
    // Speed up I/O operations for the tight time limit
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K, N;
    // Read K and N. If reading fails, exit.
    if (!(cin >> K >> N)) {
        return 0;
    }

    // Read N phone numbers as strings to handle any potential 4-digit zero-padding
    // and to easily access the first digit.
    vector<string> phones(N);
    for (int i = 0; i < N; ++i) {
        if (!(cin >> phones[i])) {
            break;
        }
    }

    // Sort numbers lexicographically (equivalent to numerical for 4-digit numbers)
    sort(phones.begin(), phones.end());

    int total_record_pages = 0;
    if (N > 0) {
        int current_group_size = 1;
        for (int i = 1; i < N; ++i) {
            // Check if the current phone number starts with the same digit as the previous one.
            // The problem states that a new first digit triggers a new page.
            if (phones[i][0] == phones[i - 1][0]) {
                current_group_size++;
            } else {
                // The first digit has changed. Calculate pages needed for the current group.
                // ceiling(current_group_size / K)
                total_record_pages += (current_group_size + K - 1) / K;
                // Start a new group with the current phone number.
                current_group_size = 1;
            }
        }
        // Add the pages required for the final group processed.
        total_record_pages += (current_group_size + K - 1) / K;
    }

    // The final answer is the 2 initial pages plus the pages used by the phone records.
    cout << 2 + total_record_pages << endl;

    return 0;
}

