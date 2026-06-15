#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    string S;
    cin >> N >> S;
    
    vector<vector<string>> rules(N + 1);
    
    // Read rules for characters 1 to N
    for(int k = 1; k <= N; k++){
        int L;
        cin >> L;
        for(int i = 0; i < L; i++){
            string token;
            cin >> token;
            rules[k].push_back(token);
        }
    }
    
    // Expand non-terminal character to final string
    map<int, string> memo;
    function<string(int)> expand = [&](int c) -> string {
        if(memo.count(c)) return memo[c];
        
        string result = "";
        for(const auto& token : rules[c]){
            if(token == "a" || token == "b"){
                result += token;
            } else {
                // Non-terminal
                int nonterm = stoi(token);
                result += expand(nonterm);
            }
            // Safety limit
            if(result.length() > 2000000) break;
        }
        return memo[c] = result;
    };
    
    string final_string = expand(N);
    
    // Count occurrences of S
    long long count = 0;
    size_t pos = 0;
    while((pos = final_string.find(S, pos)) != string::npos){
        count++;
        pos++;
    }
    
    cout << count << endl;
    
    return 0;
}
