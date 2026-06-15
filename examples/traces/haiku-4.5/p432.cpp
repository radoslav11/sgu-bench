#include<bits/stdc++.h>
using namespace std;

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string text = "";
    string line;
    while(getline(cin, line)) {
        text += line + " ";
    }
    
    vector<string> sentences;
    string current = "";
    for(size_t i = 0; i < text.length(); i++) {
        if(text[i] == '.') {
            sentences.push_back(current);
            current = "";
            if(i + 1 < text.length() && text[i+1] == ' ') i++;
        } else {
            current += text[i];
        }
    }
    if(!current.empty()) sentences.push_back(current);
    
    vector<string> results;
    
    for(const auto& sent : sentences) {
        vector<string> words;
        string word = "";
        for(char c : sent) {
            if(c == ' ') {
                if(!word.empty()) words.push_back(word);
                word = "";
            } else {
                word += c;
            }
        }
        if(!word.empty()) words.push_back(word);
        
        for(size_t i = 0; i < words.size(); i++) {
            string lower = toLower(words[i]);
            
            if(lower == "can" || lower == "may" || lower == "must" || lower == "should") {
                vector<string> modified = words;
                modified.insert(modified.begin() + i + 1, "not");
                string newSent = "";
                for(size_t j = 0; j < modified.size(); j++) {
                    newSent += modified[j];
                    if(j < modified.size() - 1) newSent += " ";
                }
                results.push_back(newSent);
            }
            
            if(lower == "is" || lower == "are") {
                vector<string> modified = words;
                int removePos = -1;
                if(i + 1 < modified.size() && (toLower(modified[i+1]) == "a" || toLower(modified[i+1]) == "the")) {
                    removePos = i + 1;
                    modified.erase(modified.begin() + removePos);
                }
                modified.insert(modified.begin() + i + 1, "no");
                string newSent = "";
                for(size_t j = 0; j < modified.size(); j++) {
                    newSent += modified[j];
                    if(j < modified.size() - 1) newSent += " ";
                }
                results.push_back(newSent);
            }
        }
    }
    
    cout << results.size() << "\n";
    for(const auto& result : results) {
        cout << result << ".\n";
    }
    
    return 0;
}
