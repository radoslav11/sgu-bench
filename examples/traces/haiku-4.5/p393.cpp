#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    cin >> N >> M;
    
    set<pair<char,char>> vocabulary;
    map<char, set<char>> conflicts; // conflicts[a] contains letters that must differ from a
    
    for(int i = 0; i < M; i++){
        string word;
        cin >> word;
        char first = word[0];
        char second = word[1];
        vocabulary.insert({first, second});
    }
    
    // Build conflict graph
    for(char a = 'a'; a < 'a' + N; a++){
        set<char> second_letters;
        set<char> first_letters;
        
        for(auto& w : vocabulary){
            if(w.first == a) second_letters.insert(w.second);
            if(w.second == a) first_letters.insert(w.first);
        }
        
        // If a appears as first letter with multiple different second letters
        for(char b : second_letters) conflicts[a].insert(b);
        // If a appears as second letter with multiple different first letters  
        for(char b : first_letters) conflicts[a].insert(b);
    }
    
    // Graph coloring with greedy approach
    map<char, int> color;
    for(char a = 'a'; a < 'a' + N; a++){
        set<int> used_colors;
        for(char conflicting : conflicts[a]){
            if(color.count(conflicting)){
                used_colors.insert(color[conflicting]);
            }
        }
        
        int c = 0;
        while(used_colors.count(c)) c++;
        color[a] = c;
    }
    
    // Group letters by color
    map<int, vector<char>> buttons;
    for(char a = 'a'; a < 'a' + N; a++){
        buttons[color[a]].push_back(a);
    }
    
    cout << buttons.size() << "\n";
    for(auto& b : buttons){
        for(char c : b.second){
            cout << c;
        }
        cout << "\n";
    }
    
    return 0;
}
