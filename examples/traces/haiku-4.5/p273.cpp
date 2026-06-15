#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int B, R, Y, W;
    cin >> B >> R >> Y >> W;
    
    map<char, vector<string>> rules;
    rules['b'].resize(B);
    rules['r'].resize(R);
    rules['y'].resize(Y);
    rules['w'].resize(W);
    
    for(int i = 0; i < B; i++) cin >> rules['b'][i];
    for(int i = 0; i < R; i++) cin >> rules['r'][i];
    for(int i = 0; i < Y; i++) cin >> rules['y'][i];
    for(int i = 0; i < W; i++) cin >> rules['w'][i];
    
    string initial;
    cin >> initial;
    
    set<string> visited;
    queue<string> q;
    set<char> winningColors;
    
    q.push(initial);
    visited.insert(initial);
    
    while(!q.empty()){
        string state = q.front();
        q.pop();
        
        // Check if this is a winning state
        if(state.length() == 1){
            winningColors.insert(state[0]);
        }
        
        // Try all possible moves
        for(int i = 0; i + 1 < state.length(); i++){
            string pair = state.substr(i, 2);
            
            // Try replacing with each color
            for(char color : {'b', 'r', 'y', 'w'}){
                for(const string& rule : rules[color]){
                    if(rule == pair){
                        // Can replace pair with color
                        string newState = state.substr(0, i) + color + state.substr(i + 2);
                        if(visited.find(newState) == visited.end()){
                            visited.insert(newState);
                            q.push(newState);
                        }
                    }
                }
            }
        }
    }
    
    string result = "";
    for(char c : {'b', 'r', 'y', 'w'}){
        if(winningColors.count(c)){
            result += c;
        }
    }
    
    if(result.empty()){
        cout << "Nobody" << endl;
    } else {
        cout << result << endl;
    }
    
    return 0;
}
