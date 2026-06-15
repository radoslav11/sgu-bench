#include <bits/stdc++.h>
using namespace std;

int main() {
    int K, N;
    cin >> K >> N;
    
    vector<int> nums(N);
    for (int i = 0; i < N; i++) {
        cin >> nums[i];
    }
    
    sort(nums.begin(), nums.end());
    
    // Group by first digit
    map<int, int> groups;
    for (int x : nums) {
        int first_digit = x / 1000;
        groups[first_digit]++;
    }
    
    int total_pages = 2; // first two pages
    
    for (auto& [digit, count] : groups) {
        int pages_needed = (count + K - 1) / K;
        total_pages += pages_needed;
    }
    
    cout << total_pages << endl;
    
    return 0;
}
