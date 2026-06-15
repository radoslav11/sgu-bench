#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    getline(cin, s);
    
    stack<char> st; // 'U' for UP, 'D' for DOWN
    
    int i = 0;
    int n = s.size();
    string result = "";
    
    while (i < n) {
        if (s[i] == '<') {
            // Read tag
            int j = i + 1;
            while (j < n && s[j] != '>') j++;
            string tag = s.substr(i+1, j-i-1); // content between < and >
            i = j + 1;
            
            if (tag == "UP") {
                st.push('U');
            } else if (tag == "DOWN") {
                st.push('D');
            } else if (tag == "/UP" || tag == "/DOWN") {
                if (!st.empty()) st.pop();
            }
        } else {
            char c = s[i];
            if (isalpha(c)) {
                if (!st.empty()) {
                    if (st.top() == 'U') {
                        c = toupper(c);
                    } else {
                        c = tolower(c);
                    }
                }
            }
            result += c;
            i++;
        }
    }
    
    cout << result << endl;
    
    return 0;
}
