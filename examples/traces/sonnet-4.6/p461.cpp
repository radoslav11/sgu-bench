#include <bits/stdc++.h>
using namespace std;

void process(vector<string> lines) {
    int n = lines.size();
    int i = 0;
    while (i < n) {
        char c = lines[i][0];
        if (c == '*' || c == '#') {
            int j = i;
            while (j < n && lines[j][0] == c) j++;
            if (j - i >= 2) {
                string tag = (c == '*') ? "ul" : "ol";
                cout << "<" << tag << ">\n";
                for (int k = i; k < j; k++) {
                    cout << "<li>\n";
                    vector<string> sub = {lines[k].substr(1)};
                    process(sub);
                    cout << "</li>\n";
                }
                cout << "</" << tag << ">\n";
            } else {
                cout << lines[i] << "\n";
            }
            i = j;
        } else {
            cout << lines[i] << "\n";
            i++;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    vector<string> lines;
    string s;
    while (getline(cin, s)) lines.push_back(s);
    process(lines);
}
