#include<bits/stdc++.h>
using namespace std;

string convert(string s);

string processFormula(string s) {
    string result = "";
    int i = 0;
    
    while(i < s.length()) {
        if(isspace(s[i])) {
            i++;
            continue;
        }
        
        // Collect consecutive letters
        if(isalpha(s[i])) {
            string letters = "";
            while(i < s.length() && isalpha(s[i])) {
                letters += s[i];
                i++;
            }
            result += "<i>" + letters + "</i>";
        }
        // Handle digits
        else if(isdigit(s[i])) {
            result += s[i];
            i++;
        }
        // Handle operators
        else if(s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
            result += "&nbsp;" + string(1, s[i]) + "&nbsp;";
            i++;
        }
        // Handle parentheses
        else if(s[i] == '(' || s[i] == ')') {
            result += s[i];
            i++;
        }
        // Handle superscript
        else if(s[i] == '^') {
            i++; // skip ^
            if(i < s.length() && s[i] == '{') {
                i++; // skip {
                string super = "";
                int braceCount = 1;
                while(i < s.length() && braceCount > 0) {
                    if(s[i] == '{') braceCount++;
                    else if(s[i] == '}') braceCount--;
                    if(braceCount > 0) super += s[i];
                    i++;
                }
                result += "<sup>" + processFormula(super) + "</sup>";
            } else {
                // Single character superscript
                string super = "";
                super += s[i];
                i++;
                result += "<sup>" + processFormula(super) + "</sup>";
            }
        }
        // Handle subscript
        else if(s[i] == '_') {
            i++; // skip _
            if(i < s.length() && s[i] == '{') {
                i++; // skip {
                string sub = "";
                int braceCount = 1;
                while(i < s.length() && braceCount > 0) {
                    if(s[i] == '{') braceCount++;
                    else if(s[i] == '}') braceCount--;
                    if(braceCount > 0) sub += s[i];
                    i++;
                }
                result += "<sub>" + processFormula(sub) + "</sub>";
            } else {
                // Single character subscript
                string sub = "";
                sub += s[i];
                i++;
                result += "<sub>" + processFormula(sub) + "</sub>";
            }
        }
        else {
            i++;
        }
    }
    
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    while(getline(cin, line)) {
        // Find content between dollar signs
        int first = line.find('$');
        int last = line.rfind('$');
        
        if(first != string::npos && last != string::npos && first < last) {
            string formula = line.substr(first + 1, last - first - 1);
            cout << processFormula(formula) << "\n";
        }
    }
    
    return 0;
}
