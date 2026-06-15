#include<bits/stdc++.h>
using namespace std;

int main(){
    string input, line;
    while(getline(cin, line)) input += " " + line;
    
    map<string, set<string>> grammar;
    
    int n = input.size();
    int i = 0;
    
    auto skipWS = [&]() {
        while(i < n && isspace(input[i])) i++;
    };
    
    auto readWord = [&]() {
        skipWS();
        string s;
        while(i < n && isalnum(input[i])) s += tolower(input[i++]);
        return s;
    };
    
    auto readChar = [&]() {
        skipWS();
        i++; // '
        char c = input[i++];
        i++; // '
        return c;
    };
    
    function<void(string, string)> parseSegs = [&](string rhs, string routine) {
        skipWS();
        while(i < n) {
            string w = readWord();
            if(w == "end") break;
            if(w == "if") {
                readWord(); // Peek
                skipWS();
                i++; // =
                char c = readChar();
                readWord(); // Then
                readWord(); // Begin
                parseSegs(rhs + "'" + c + "'", routine);
                skipWS();
                i++; // End
                skipWS();
                if(input[i] == 'E') {
                    readWord(); // Else
                    skipWS();
                    if(input[i] == 'I') {
                        parseSegs(rhs, routine);
                    } else {
                        readWord(); // Error
                    }
                }
                skipWS();
                i++; // ;
            } else {
                rhs += "<" + w + ">";
                skipWS();
                i++; // ;
                grammar[routine].insert(rhs);
            }
        }
        skipWS();
        i += 3; // End
        skipWS();
        i++; // ;
    };
    
    i = 0;
    while(i < n) {
        string w = readWord();
        if(w == "procedure") {
            string name = readWord();
            skipWS();
            i++; // ;
            skipWS();
            string next = readWord();
            if(next != "forward") {
                i -= next.length();
                readWord(); // Begin
                parseSegs("", name);
            }
        }
    }
    
    for(auto& [k, v] : grammar) {
        cout << "<" << k << ">::=";
        bool first = true;
        for(auto& s : v) {
            if(!first) cout << "|";
            cout << s;
            first = false;
        }
        cout << "\n";
    }
    
    return 0;
}
