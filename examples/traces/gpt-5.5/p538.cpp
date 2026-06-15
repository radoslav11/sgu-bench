#include <bits/stdc++.h>
using namespace std;

bool isLetterOrSpace(char c) {
    return c == ' ' || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    int totalBrackets = 0;
    int nonEmoticonBrackets = 0;

    int prevBracketPos = -1;
    char prevBracket = 0;
    bool cleanSincePrevBracket = true;

    for (int i = 0; i < (int)s.size(); ++i) {
        char c = s[i];

        if (c == '(' || c == ')') {
            totalBrackets++;

            if (prevBracketPos != -1) {
                if (prevBracket == '(' && c == ')' && cleanSincePrevBracket) {
                    nonEmoticonBrackets += 2;
                }
            }

            prevBracketPos = i;
            prevBracket = c;
            cleanSincePrevBracket = true;
        } else {
            if (!isLetterOrSpace(c)) {
                cleanSincePrevBracket = false;
            }
        }
    }

    cout << totalBrackets - nonEmoticonBrackets << '\n';

    return 0;
}
