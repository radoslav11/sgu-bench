#include <bits/stdc++.h>
using namespace std;

bool isSubsequence(const string& text, int start, int end, const string& message) {
    int msgIdx = 0;
    for (int i = start; i <= end && msgIdx < (int)message.length(); i++) {
        if (text[i] == message[msgIdx]) {
            msgIdx++;
        }
    }
    return msgIdx == (int)message.length();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string text, message;
    cin >> text >> message;
    
    long long count = 0;
    int n = text.length();
    
    // Try all possible substrings text[i..j]
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (isSubsequence(text, i, j, message)) {
                count++;
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
