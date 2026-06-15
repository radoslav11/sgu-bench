#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Valid email address grammar:
 * <letter> ::= a|b|...|z|A|B|...|Z
 * <symbol> ::= <letter>|0|1|...|9|_|-
 * <word> ::= <symbol>|<symbol><word>
 * <prefix> ::= <word>|<prefix>.<word>
 * <domain> ::= <letter><letter>|<letter><letter><letter>
 * <suffix> ::= <prefix>.<domain>
 * <address> ::= <prefix>@<suffix>
 */

// Check if a character is a letter (A-Z, a-z)
bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Check if a character is a symbol (letter, digit, underscore, hyphen)
bool is_symbol(char c) {
    return is_letter(c) || (c >= '0' && c <= '9') || c == '_' || c == '-';
}

// Check if a string is a word (non-empty sequence of symbols)
bool is_word(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!is_symbol(c)) return false;
    }
    return true;
}

// Check if a string is a prefix (one or more words separated by dots)
bool is_prefix(const string& s) {
    if (s.empty()) return false;
    size_t start = 0;
    size_t end = s.find('.');
    while (end != string::npos) {
        // Check if the part before the dot is a valid word
        if (!is_word(s.substr(start, end - start))) return false;
        start = end + 1;
        end = s.find('.', start);
    }
    // Check if the final part is a valid word
    return is_word(s.substr(start));
}

// Check if a string is a domain (exactly 2 or 3 letters)
bool is_domain(const string& s) {
    if (s.length() != 2 && s.length() != 3) return false;
    for (char c : s) {
        if (!is_letter(c)) return false;
    }
    return true;
}

// Check if a string is a suffix (a prefix followed by a dot and a domain)
bool is_suffix(const string& s) {
    size_t last_dot = s.find_last_of('.');
    if (last_dot == string::npos) return false;
    string prefix_part = s.substr(0, last_dot);
    string domain_part = s.substr(last_dot + 1);
    return is_prefix(prefix_part) && is_domain(domain_part);
}

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;
    
    string dummy;
    getline(cin, dummy); // Consume the leftover newline after N

    while (N--) {
        string line;
        if (!getline(cin, line)) break;

        // Check if address contains exactly one '@' symbol
        int at_count = 0;
        int at_pos = -1;
        for (int i = 0; i < (int)line.length(); ++i) {
            if (line[i] == '@') {
                at_count++;
                at_pos = i;
            }
        }

        if (at_count != 1) {
            cout << "NO" << endl;
            continue;
        }

        // Split the line into prefix and suffix based on the '@' position
        string prefix_part = line.substr(0, at_pos);
        string suffix_part = line.substr(at_pos + 1);

        // Validate both prefix and suffix parts
        if (is_prefix(prefix_part) && is_suffix(suffix_part)) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }

    return 0;
}

