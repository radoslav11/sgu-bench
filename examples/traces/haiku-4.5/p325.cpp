#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    
    int n = s.length();
    
    // Check if palindrome is possible
    map<char, int> freq;
    for(char c : s) {
        freq[c]++;
    }
    
    int oddCount = 0;
    for(auto p : freq) {
        if(p.second % 2 == 1) {
            oddCount++;
        }
    }
    
    // If more than 1 character has odd frequency, palindrome is impossible
    if(oddCount > 1) {
        cout << -1 << endl;
        return 0;
    }
    
    long long swaps = 0;
    int left = 0, right = n - 1;
    
    while(left < right) {
        if(s[left] == s[right]) {
            left++;
            right--;
        }
        else {
            // Find s[left] from the right side
            int k = right;
            while(k > left && s[k] != s[left]) {
                k--;
            }
            
            // Find s[right] from the left side
            int j = left;
            while(j < right && s[j] != s[right]) {
                j++;
            }
            
            // Choose which one to move: prefer moving the one that's closer
            // or we can use a consistent strategy
            if(right - k <= j - left) {
                // Move s[k] to position right by swapping with adjacent elements
                while(k < right) {
                    swap(s[k], s[k+1]);
                    k++;
                    swaps++;
                }
            }
            else {
                // Move s[j] to position left by swapping with adjacent elements
                while(j > left) {
                    swap(s[j], s[j-1]);
                    j--;
                    swaps++;
                }
            }
            
            left++;
            right--;
        }
    }
    
    cout << swaps << endl;
    
    return 0;
}
