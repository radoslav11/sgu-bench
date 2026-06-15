#include<bits/stdc++.h>
using namespace std;

int main(){
    int q, c;
    cin >> q >> c;
    cin.ignore();
    
    string s;
    getline(cin, s);
    
    set<char> replaceable = {'A', 'B', 'C', 'E', 'H', 'K', 'M', 'O', 'P', 'T', 'X',
                             'a', 'c', 'e', 'o', 'p', 'x', 'y'};
    
    vector<string> words;
    string current = "";
    for(char ch : s) {
        if(ch == ' ') {
            if(!current.empty()) {
                words.push_back(current);
                current = "";
            }
        } else {
            current += ch;
        }
    }
    if(!current.empty()) words.push_back(current);
    
    int replaceableCount = 0;
    int letterCount = 0;
    for(const auto& word : words) {
        for(char ch : word) {
            letterCount++;
            if(replaceable.count(ch)) replaceableCount++;
        }
    }
    
    long long replacements = 1LL << replaceableCount;
    
    int gaps = max(0, (int)words.size() - 1);
    long long spaces = 0;
    for(int totalSpaces = 0; totalSpaces <= c - q; totalSpaces++) {
        int betweenSpaces = totalSpaces;
        if(gaps > 0) {
            if(betweenSpaces >= gaps) {
                int extra = betweenSpaces - gaps;
                long long ways = 1;
                for(int i = 0; i < gaps + 1; i++) {
                    ways = ways * (extra + gaps + 1 - i) / (i + 1);
                }
                spaces += ways;
            }
        } else {
            spaces++;
        }
    }
    
    cout << replacements * spaces << endl;
    
    return 0;
}
