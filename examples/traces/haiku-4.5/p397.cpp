#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    
    string word = "";
    int cursor = 0;
    
    for(char c : s) {
        if(c >= 'a' && c <= 'z') {
            word.insert(cursor, 1, c);
            cursor++;
        }
        else if(c == 'L') {
            if(cursor > 0) cursor--;
        }
        else if(c == 'R') {
            if(cursor < word.length()) cursor++;
        }
    }
    
    cout << word << endl;
    
    return 0;
}
