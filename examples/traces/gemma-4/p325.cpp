#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

/**
 * Problem: Palindrome
 * Task: Find the minimum number of adjacent swaps to make a string a palindrome.
 * Approach:
 * 1. Check if the string can form a palindrome (at most one character has an odd frequency).
 * 2. Use a greedy strategy to determine the target positions of each character in the optimal palindrome.
 *    - For each character, pair its first occurrence in the original string with its last occurrence.
 *    - Match the first occurrence to the next available position from the left, 
 *      and the last occurrence to the next available position from the right.
 *    - If a character has an odd frequency, its middle occurrence will go to the middle of the available positions.
 * 3. The minimum number of adjacent swaps is equal to the number of inversions in the array of target positions.
 * 4. Time Complexity: O(N log N), where N is the length of the string.
 * 5. Space Complexity: O(N) to store positions and the Fenwick tree.
 */

typedef long long ll;

// Fenwick Tree for inversion counting
struct FenwickTree {
    int n;
    vector<int> tree;
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}
    void update(int i, int delta) {
        for (++i; i <= n; i += i & -i) tree[i] += delta;
    }
    int query(int i) {
        int sum = 0;
        for (++i; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

int main() {
    // Faster I/O
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    int n = s.size();
    
    vector<int> count(26, 0);
    vector<vector<int>> pos(26);
    for (int i = 0; i < n; ++i) {
        count[s[i] - 'A']++;
        pos[s[i] - 'A'].push_back(i);
    }

    // Check if a palindrome is possible
    int odd_counts = 0;
    for (int i = 0; i < 26; ++i) {
        if (count[i] % 2 != 0) odd_counts++;
    }

    if (odd_counts > 1) {
        cout << -1 << endl;
        return 0;
    }

    // target_pos[i] will store the index in the target palindrome for the character at s[i]
    vector<int> target_pos(n);
    vector<bool> is_used(n, false);
    deque<int> available_positions;
    for (int i = 0; i < n; ++i) {
        available_positions.push_back(i);
    }

    // Pointers for iterating through character positions
    vector<int> pos_idx_front(26, 0);
    vector<int> pos_idx_back(26);
    for (int i = 0; i < 26; ++i) {
        pos_idx_back[i] = (int)pos[i].size() - 1;
    }

    // Greedy assignment of target positions
    for (int i = 0; i < n; ++i) {
        if (is_used[i]) continue;
        int c = s[i] - 'A';
        int current_size = pos_idx_back[c] - pos_idx_front[c] + 1;
        
        // If the character has an odd number of occurrences left and it's the only one
        if (current_size % 2 != 0 && pos_idx_front[c] == pos_idx_back[c]) {
            int mid_idx_in_available = available_positions.size() / 2;
            target_pos[i] = available_positions[mid_idx_in_available];
            
            // Deleting the middle element from the deque is O(N), but occurs at most once
            auto it = available_positions.begin() + mid_idx_in_available;
            available_positions.erase(it);
            
            is_used[i] = true;
            pos_idx_front[c]++;
        } else {
            // Pair the current character with its last available occurrence
            int k = pos[c][pos_idx_back[c]];
            target_pos[i] = available_positions.front();
            available_positions.pop_front();
            target_pos[k] = available_positions.back();
            available_positions.pop_back();
            
            is_used[i] = true;
            is_used[k] = true;
            pos_idx_front[c]++;
            pos_idx_back[c]--;
        }
    }

    // Counting inversions in target_pos using Fenwick Tree
    FenwickTree ft(n);
    ll inversions = 0;
    for (int i = n - 1; i >= 0; --i) {
        inversions += ft.query(target_pos[i] - 1);
        ft.update(target_pos[i], 1);
    }

    cout << inversions << endl;

    return 0;
}

