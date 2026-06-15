#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    cin.ignore();
    
    for(int test = 0; test < n; test++){
        string line;
        getline(cin, line);
        
        // Remove quotes
        if(line.front() == '"') line = line.substr(1);
        if(line.back() == '"') line = line.substr(0, line.length()-1);
        
        vector<string> tokens;
        string current = "";
        bool inWord = false;
        
        // Tokenize
        for(char c : line){
            if(isalpha(c)){
                if(!inWord && !current.empty()){
                    tokens.push_back(current);
                    current = "";
                }
                inWord = true;
                current += c;
            }
            else if(c == '-'){
                if(inWord){
                    tokens.push_back(current);
                    current = "";
                    inWord = false;
                }
                current += c;
            }
            else { // space
                if(!current.empty()){
                    tokens.push_back(current);
                    current = "";
                }
                inWord = false;
            }
        }
        if(!current.empty()){
            tokens.push_back(current);
        }
        
        // Validate and format
        string result = "";
        bool error = false;
        
        for(int i = 0; i < tokens.size(); i++){
            string token = tokens[i];
            
            if(isalpha(token[0])){ // word
                if(!result.empty() && result.back() != ' ' && result.back() != '-'){
                    result += " ";
                }
                result += token;
            }
            else { // dash sequence
                int dashCount = token.length();
                
                if(dashCount == 1){ // hyphen
                    // Check if surrounded by letters
                    bool prevLetter = !result.empty() && isalpha(result.back());
                    bool nextLetter = (i+1 < tokens.size()) && isalpha(tokens[i+1][0]);
                    
                    if(prevLetter && nextLetter){
                        result += "-";
                    } else {
                        error = true;
                        break;
                    }
                }
                else if(dashCount == 2 || dashCount == 3){
                    // Need spaces around
                    bool prevSpace = result.empty() || result.back() == ' ';
                    bool nextSpace = (i+1 >= tokens.size()) || isalpha(tokens[i+1][0]);
                    
                    if(!prevSpace) result += " ";
                    
                    for(int j = 0; j < dashCount; j++) result += "-";
                    
                    if(i+1 < tokens.size()) result += " ";
                }
                else {
                    error = true;
                    break;
                }
            }
        }
        
        if(error || result.empty() || result.back() == ' '){
            cout << "error\n";
        }
        else {
            cout << "\"" << result << "\"\n";
        }
    }
    
    return 0;
}
