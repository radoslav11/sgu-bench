#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

/**
 * The problem asks for converting TeX-like mathematical formulas to HTML.
 * The components of the formula are:
 * - Latin letters (maximal sequences are wrapped in <i> tags)
 * - Digits (sequences are kept as-is)
 * - Binary operators (+, -, *, /) (surrounded by &nbsp;)
 * - Parentheses (kept as-is)
 * - Superscripts (^ followed by single char or {content}) (wrapped in <sup>)
 * - Subscripts (_ followed by single char or {content}) (wrapped in <sub>)
 * 
 * We can use a recursive descent parser. Since the input size is relatively small (128 KB)
 * and mathematical formula nesting is typically shallow, the recursive substr-based approach
 * will be efficient enough.
 */

string parse(string s) {
    string res = "";
    for (int i = 0; i < (int)s.length(); ) {
        if (isalpha((unsigned char)s[i])) {
            string letters = "";
            while (i < (int)s.length() && isalpha((unsigned char)s[i])) {
                letters += s[i++];
            }
            res += "<i>" + letters + "</i>";
        } else if (isdigit((unsigned char)s[i])) {
            string digits = "";
            while (i < (int)s.length() && isdigit((unsigned char)s[i])) {
                digits += s[i++];
            }
            res += digits;
        } else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
            res += "&nbsp;";
            res += s[i];
            res += "&nbsp;";
            i++;
        } else if (s[i] == '(' || s[i] == ')') {
            res += s[i++];
        } else if (s[i] == '^') {
            i++;
            res += "<sup>";
            if (i < (int)s.length() && s[i] == '{') {
                int start = i + 1;
                int count = 1;
                i++;
                while (i < (int)s.length() && count > 0) {
                    if (s[i] == '{') count++;
                    else if (s[i] == '}') count--;
                    i++;
                }
                // i is now at the position after the closing brace '}'
                if (count == 0) {
                    // i-1 is the index of '}'
                    res += parse(s.substr(start, i - start - 1));
                } else {
                    // This should not happen given the problem statement about correct formulas
                }
            } else {
                // Single character superscript
                if (i < (int)s.length()) {
                    string single = "";
                    single += s[i++];
                    res += parse(single);
                }
            }
            res += "</sup>";
        } else if (s[i] == '_') {
            i++;
            res += "<sub>";
            if (i < (int)s.length() && s[i] == '{') {
                int start = i + 1;
                int count = 1;
                i++;
                while (i < (int)s.length() && count > 0) {
                    if (s[i] == '{') count++;
                    else if (s[i] == '}') count--;
                    i++;
                }
                if (count == 0) {
                    res += parse(s.substr(start, i - start - 1));
                }
            } else {
                // Single character subscript
                if (i < (int)s.length()) {
                    string single = "";
                    single += s[i++];
                    res += parse(single);
                }
            }
            res += "</sub>";
        } else {
            // Fallback for any unexpected character
            i++;
        }
    }
    return res;
}

int main() {
    // Speed up input and output
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;

        // Find the formula enclosed within dollar signs
        size_t first_dollar = line.find('$');
        size_t last_dollar = line.rfind('$');

        if (first_dollar != string::npos && last_dollar != string::npos && first_dollar < last_dollar) {
            string formula = line.substr(first_dollar + 1, last_dollar - first_dollar - 1);

            // Ignore all whitespace characters in the formula
            string stripped = "";
            for (char c : formula) {
                if (!isspace((unsigned char)c)) {
                    stripped += c;
                }
            }

            // Parse and print the HTML version
            cout << parse(stripped) << "\n";
        }
    }

    return 0;
}

