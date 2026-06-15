#include <iostream>
#include <string>
#include <cctype>
#include <vector>

bool isLetter(char c) {
    return std::isalpha(static_cast<unsigned char>(c));
}

bool isSymbol(char c) {
    return isLetter(c) || std::isdigit(static_cast<unsigned char>(c)) || c == '_' || c == '-';
}

bool isWord(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isSymbol(c)) return false;
    }
    return true;
}

bool isPrefix(const std::string& s) {
    if (s.empty()) return false;
    
    // Split by dots and check each part is a word
    size_t start = 0;
    size_t pos = 0;
    while (pos <= s.size()) {
        if (pos == s.size() || s[pos] == '.') {
            std::string part = s.substr(start, pos - start);
            if (!isWord(part)) return false;
            start = pos + 1;
        }
        pos++;
    }
    
    // Check that it doesn't end with a dot
    return s.back() != '.';
}

bool isDomain(const std::string& s) {
    if (s.empty()) return false;
    
    // Split by dots and check each part is a letter sequence
    size_t start = 0;
    size_t pos = 0;
    while (pos <= s.size()) {
        if (pos == s.size() || s[pos] == '.') {
            std::string part = s.substr(start, pos - start);
            if (part.empty() || part.size() < 2 || part.size() > 3) return false;
            for (char c : part) {
                if (!isLetter(c)) return false;
            }
            start = pos + 1;
        }
        pos++;
    }
    
    // Check that it doesn't end with a dot
    return s.back() != '.';
}

bool isSuffix(const std::string& s) {
    // <suffix> ::= <prefix>.<domain>
    // Find the last dot
    size_t lastDot = s.rfind('.');
    if (lastDot == std::string::npos || lastDot == 0 || lastDot == s.size() - 1) {
        return false;
    }
    
    std::string prefixPart = s.substr(0, lastDot);
    std::string domainPart = s.substr(lastDot + 1);
    
    return isPrefix(prefixPart) && isDomain(domainPart);
}

bool isValidEmail(const std::string& s) {
    // <address> ::= <prefix>@<suffix>
    size_t atPos = s.find('@');
    if (atPos == std::string::npos || atPos == 0 || atPos == s.size() - 1) {
        return false;
    }
    
    std::string prefixPart = s.substr(0, atPos);
    std::string suffixPart = s.substr(atPos + 1);
    
    return isPrefix(prefixPart) && isSuffix(suffixPart);
}

int main() {
    int n;
    std::cin >> n;
    std::cin.ignore(); // Skip newline after reading n
    
    for (int i = 0; i < n; i++) {
        std::string email;
        std::getline(std::cin, email);
        
        if (isValidEmail(email)) {
            std::cout << "YES" << std::endl;
        } else {
            std::cout << "NO" << std::endl;
        }
    }
    
    return 0;
}
