#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);
    
    int n = s.size();
    vector<int> count(26, 0);
    
    // Count frequency of each character
    for (char c : s) {
        count[c - 'A']++;
    }
    
    // Check if palindrome is possible
    int oddCount = 0;
    for (int i = 0; i < 26; i++) {
        if (count[i] % 2 != 0) {
            oddCount++;
        }
    }
    
    // For a palindrome, at most one character can have odd frequency
    if (oddCount > 1) {
        cout << -1 << '\n';
        return 0;
    }
    
    // Create array for positions of each character
    vector<vector<int>> positions(26);
    for (int i = 0; i < n; i++) {
        positions[s[i] - 'A'].push_back(i);
    }
    
    // Build the target palindrome from outside to inside
    // We'll simulate building the palindrome by matching pairs
    // For efficiency, we use a greedy approach: 
    // For each position from left to right (up to n/2), 
    // find the matching character for the current left position
    
    // However, a more efficient method for counting swaps:
    // Use the method of converting to permutation problem.
    // First, determine the final positions of characters in the palindrome.
    // Then count inversions needed to achieve that permutation.
    
    // Build the target palindrome string
    string target(n, ' ');
    
    int left = 0, right = n - 1;
    vector<int> used(n, 0); // mark used positions
    
    // For each character, we need to place pairs in the palindrome
    // We'll determine the final arrangement of characters
    
    // Alternative approach: 
    // 1. Determine the final positions for each character in the palindrome
    // 2. Create an array of the original indices in the order they appear in the target palindrome
    // 3. Count the minimum adjacent swaps to achieve this permutation = number of inversions
    
    // Let's construct the target palindrome arrangement:
    // For characters with even count, they all go in pairs
    // For the character with odd count (if any), one goes in the middle
    
    vector<int> finalPositions(n);
    int pos = 0;
    vector<bool> processed(26, false);
    
    // First, place pairs from outside in
    for (int i = 0; i < 26; i++) {
        if (count[i] == 0) continue;
        if (count[i] % 2 == 1) {
            // This character has odd count, so one goes in the middle
            // But we'll handle it later, for now process pairs
            // Actually, let's process all characters by their frequency
        }
    }
    
    // Better approach: build the target palindrome character by character
    // But since we only need the number of swaps, we can avoid constructing the actual palindrome
    // and just compute the minimum number of adjacent swaps to rearrange the string into some palindrome
    
    // Known approach:
    // 1. If impossible, return -1
    // 2. Otherwise, use two pointers from both ends
    // 3. For the left pointer, find the matching character from the right side
    // 4. Move it to the symmetric position with minimum swaps (by counting swaps needed to bring it)
    
    // This greedy approach works because we want minimum swaps
    // We simulate the process of building the palindrome from both ends
    
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = s[i] - 'A';
    }
    
    long long swaps = 0;
    int i = 0, j = n - 1;
    
    while (i < j) {
        // Find matching character for arr[i] from the right side
        int k = j;
        while (k > i && arr[k] != arr[i]) {
            k--;
        }
        
        if (k == i) {
            // No matching character found, this means arr[i] is the odd character
            // Swap it with the next character and count one swap
            // But since we might need to move it all the way to center, 
            // we'll handle it by moving it one position and continue
            swap(arr[i], arr[i + 1]);
            swaps++;
            continue;
        } else if (k == j) {
            // Found match at j, no swaps needed for this pair
            i++;
            j--;
        } else {
            // Found match at k, need to move it to j
            // Move it from k to j by swapping adjacent elements
            for (int l = k; l < j; l++) {
                swap(arr[l], arr[l + 1]);
                swaps++;
            }
            i++;
            j--;
        }
    }
    
    cout << swaps << '\n';
    
    return 0;
}
