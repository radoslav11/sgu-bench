#include<bits/stdc++.h>
using namespace std;

map<pair<string, char>, bool> memo;

bool solve(string s, char currentPlayer, char startingPlayer) {
    // Base case: single empty stand of the starting player
    if(s.length() == 1) {
        return s[0] == '?' && currentPlayer == startingPlayer;
    }
    
    auto key = make_pair(s, currentPlayer);
    if(memo.find(key) != memo.end()) {
        return memo[key];
    }
    
    char opponent = (currentPlayer == 'A') ? 'B' : 'A';
    bool result = false;
    
    // Option 1: Fill an empty stand with currentPlayer's production
    for(int i = 0; i < (int)s.length(); i++) {
        if(s[i] == currentPlayer) {
            string prev = s.substr(0, i) + '?' + s.substr(i+1);
            if(solve(prev, opponent, startingPlayer)) {
                result = true;
                break;
            }
        }
    }
    
    // Option 2: Insert currentPlayer + opponent empty stand
    if(!result) {
        for(int i = 0; i < (int)s.length() - 1; i++) {
            if(s[i] == currentPlayer && s[i+1] == opponent) {
                string prev = s.substr(0, i) + '?' + s.substr(i+2);
                if(solve(prev, opponent, startingPlayer)) {
                    result = true;
                    break;
                }
            }
        }
    }
    
    memo[key] = result;
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    char startCompany;
    string stands;
    cin >> startCompany >> stands;
    
    if(solve(stands, startCompany, startCompany)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
