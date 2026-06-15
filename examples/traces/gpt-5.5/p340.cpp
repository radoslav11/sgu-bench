#include <bits/stdc++.h>
using namespace std;

struct Parser {
    string s;
    int pos = 0;

    Parser(const string& str) : s(str) {}

    bool isLetter(char c) {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
    }

    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    string parseUntil(char stop = 0) {
        string res;

        while (pos < (int)s.size()) {
            char c = s[pos];

            if (stop && c == stop) {
                pos++;
                break;
            }

            if (isLetter(c)) {
                string letters;
                while (pos < (int)s.size() && isLetter(s[pos])) {
                    letters += s[pos++];
                }
                res += "<i>" + letters + "</i>";
            } else if (isOperator(c)) {
                res += "&nbsp;";
                res += c;
                res += "&nbsp;";
                pos++;
            } else if (c == '^' || c == '_') {
                char type = c;
                pos++;

                if (type == '^') res += "<sup>";
                else res += "<sub>";

                if (pos < (int)s.size() && s[pos] == '{') {
                    pos++;
                    res += parseUntil('}');
                } else {
                    res += parseOne();
                }

                if (type == '^') res += "</sup>";
                else res += "</sub>";
            } else {
                res += c;
                pos++;
            }
        }

        return res;
    }

    string parseOne() {
        if (pos >= (int)s.size()) return "";

        char c = s[pos++];

        if (isLetter(c)) {
            string res = "<i>";
            res += c;
            res += "</i>";
            return res;
        }

        if (isOperator(c)) {
            string res = "&nbsp;";
            res += c;
            res += "&nbsp;";
            return res;
        }

        return string(1, c);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        size_t l = line.find('$');
        size_t r = line.rfind('$');

        if (l == string::npos || r == string::npos || l == r) {
            continue;
        }

        string formula = line.substr(l + 1, r - l - 1);
        string compact;

        for (char c : formula) {
            if (!isspace((unsigned char)c)) {
                compact += c;
            }
        }

        Parser parser(compact);
        cout << parser.parseUntil() << '\n';
    }

    return 0;
}
