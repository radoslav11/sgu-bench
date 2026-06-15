#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

/**
 * Problem: 324. The Text Formatting
 * 
 * The goal is to format a text with words, spaces, and hyphens ('-').
 * Rules:
 * - Word: Sequence of Latin letters.
 * - Hyphen: '-' used as 'word-word' (no spaces).
 * - Short dash: '--' used as ' -- ' (spaces on both sides).
 * - Long dash: '---' used as ' --- ' (spaces on both sides).
 * - If no '-' between words, use a single space.
 * - Any other space characters are not allowed.
 * - If impossible to format, output "error".
 * 
 * Strategy:
 * 1. Parse the input into tokens: WORD (sequence of letters) or DASH_BLOCK (one or more '-' characters).
 * 2. Check for error conditions (e.g., a single '-' cannot be at the start, end, or not between words).
 * 3. Construct the output string by following rules for hyphen, short dash, and long dash.
 */

struct Token {
    bool isWord;
    string value;
    int k; // number of dashes if it's a DASH_BLOCK
};

// Formats a sequence of k dashes into short (--) and long (---) dashes.
string formatted_dashes(int k) {
    int a = 0, b = 0;
    // Find non-negative integers a and b such that 2a + 3b = k.
    // This is always possible for k >= 2.
    for (int b_val = k / 3; b_val >= 0; --b_val) {
        if ((k - 3 * b_val) % 2 == 0) {
            b = b_val;
            a = (k - 3 * b_val) / 2;
            break;
        }
    }
    string res = "";
    bool first = true;
    for (int i = 0; i < a; ++i) {
        if (!first) res += " ";
        res += "--";
        first = false;
    }
    for (int i = 0; i < b; ++i) {
        if (!first) res += " ";
        res += "---";
        first = false;
    }
    return res;
}

void solve() {
    string input;
    if (!getline(cin, input) || input.empty()) return;
    int N = stoi(input);
    while (N--) {
        string line;
        getline(cin, line);
        // Strip quotes. The quoted text is within "..."
        if (line.length() >= 2 && line.front() == '"' && line.back() == '"') {
            line = line.substr(1, line.length() - 2);
        }

        vector<Token> tokens;
        int m = line.length();
        for (int i = 0; i < m; ) {
            if (isalpha(line[i])) {
                string w = "";
                while (i < m && isalpha(line[i])) {
                    w += line[i++];
                }
                tokens.push_back({true, w, 0});
            } else if (line[i] == '-') {
                int count = 0;
                while (i < m && (line[i] == '-' || line[i] == ' ')) {
                    if (line[i] == '-') count++;
                    i++;
                }
                tokens.push_back({false, "", count});
            } else {
                i++;
            }
        }

        bool error = false;
        int numTokens = tokens.size();
        // Error check: a single '-' (k=1) must be between two words.
        for (int i = 0; i < numTokens; ++i) {
            if (!tokens[i].isWord && tokens[i].k == 1) {
                if (i == 0 || i == numTokens - 1 || tokens[i - 1].isWord == false || tokens[i + 1].isWord == false) {
                    error = true;
                    break;
                }
            }
        }

        if (error) {
            cout << "error" << endl;
        } else {
            string res = "";
            for (int i = 0; i < numTokens; ++i) {
                if (tokens[i].isWord) {
                    if (i > 0) {
                        if (tokens[i - 1].isWord) {
                            res += " ";
                        } else if (tokens[i - 1].k == 1) {
                            res += "-";
                        } else if (tokens[i - 1].k >= 2) {
                            // DASH_BLOCK with k>=2 already added a trailing space.
                        }
                    }
                    res += tokens[i].value;
                } else { // DASH_BLOCK
                    if (tokens[i].k == 1) continue; // Handled as a hyphen in the WORD branch
                    // For k >= 2
                    if (i > 0 && tokens[i - 1].isWord) res += " "; // space to the left
                    res += formatted_dashes(tokens[i].k);
                    if (i < numTokens - 1) res += " "; // space to the right
                }
            }
            cout << "\"" << res << "\"" << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

