#include <bits/stdc++.h>
using namespace std;

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isSymbol(char c) {
    return isLetter(c) || (c >= '0' && c <= '9') || c == '_' || c == '-';
}

bool isWord(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isSymbol(c)) return false;
    }
    return true;
}

bool isPrefix(const string& s) {
    if (s.empty()) return false;
    
    // Split by dots
    vector<string> words;
    string current = "";
    for (char c : s) {
        if (c == '.') {
            words.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    words.push_back(current);
    
    // Each word must be valid
    for (const string& w : words) {
        if (!isWord(w)) return false;
    }
    return true;
}

bool isDomain(const string& s) {
    if (s.length() != 2 && s.length() != 3) return false;
    for (char c : s) {
        if (!isLetter(c)) return false;
    }
    return true;
}

bool isSuffix(const string& s) {
    // Must be prefix.domain
    int lastDot = s.rfind('.');
    if (lastDot == string::npos || lastDot == 0 || lastDot == s.length() - 1) {
        return false;
    }
    
    string prefixPart = s.substr(0, lastDot);
    string domainPart = s.substr(lastDot + 1);
    
    return isPrefix(prefixPart) && isDomain(domainPart);
}

bool isValidAddress(const string& s) {
    int atCount = 0;
    int atPos = -1;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '@') {
            atCount++;
            atPos = i;
        }
    }
    
    if (atCount != 1) return false;
    if (atPos == 0 || atPos == s.length() - 1) return false;
    
    string prefixPart = s.substr(0, atPos);
    string suffixPart = s.substr(atPos + 1);
    
    return isPrefix(prefixPart) && isSuffix(suffixPart);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    cin.ignore();
    
    for (int i = 0; i < n; i++) {
        string email;
        getline(cin, email);
        
        if (isValidAddress(email)) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    
    return 0;
}
