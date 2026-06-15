#include<bits/stdc++.h>
using namespace std;

bool isPalindrome(const string& s) {
    int n = s.length();
    for(int i = 0; i < n/2; i++) {
        if(s[i] != s[n-1-i]) return false;
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s1, s2;
    cin >> s1 >> s2;
    
    string result = "";
    int maxLen = 0;
    
    // Try all substrings of s1
    for(int i = 0; i < s1.length(); i++) {
        for(int j = i; j < s1.length(); j++) {
            string substring = s1.substr(i, j - i + 1);
            
            // Check if it's a palindrome
            if(isPalindrome(substring)) {
                // Check if it exists in s2
                if(s2.find(substring) != string::npos) {
                    if(substring.length() > maxLen) {
                        maxLen = substring.length();
                        result = substring;
                    }
                }
            }
        }
    }
    
    cout << result << endl;
    
    return 0;
}
