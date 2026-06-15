#include<bits/stdc++.h>
using namespace std;

int main(){
    string s;
    cin >> s;
    
    int n = s.length();
    
    // State: (position, stack_state, initial_dishes_needed)
    map<pair<int, vector<char>>, int> best; // best initial count for each state
    
    queue<tuple<int, vector<char>, int>> q;
    q.push({0, {}, 0});
    best[{0, {}}] = 0;
    
    int result = -1;
    
    while(!q.empty()) {
        auto [pos, st, initial] = q.front();
        q.pop();
        
        if(pos == n) {
            if(st.empty()) {
                if(result == -1 || initial < result) {
                    result = initial;
                }
            }
            continue;
        }
        
        if(s[pos] == '*') {
            // Try no action
            auto key = make_pair(pos + 1, st);
            if(best.find(key) == best.end() || best[key] > initial) {
                best[key] = initial;
                q.push({pos + 1, st, initial});
            }
            
            // Try adding each letter
            for(char c = 'a'; c <= 'z'; c++) {
                vector<char> st2 = st;
                st2.push_back(c);
                auto key = make_pair(pos + 1, st2);
                if(best.find(key) == best.end() || best[key] > initial) {
                    best[key] = initial;
                    q.push({pos + 1, st2, initial});
                }
            }
            
            // Try removing
            if(!st.empty()) {
                vector<char> st2 = st;
                st2.pop_back();
                auto key = make_pair(pos + 1, st2);
                if(best.find(key) == best.end() || best[key] > initial) {
                    best[key] = initial;
                    q.push({pos + 1, st2, initial});
                }
            }
        }
        else if(islower(s[pos])) {
            vector<char> st2 = st;
            st2.push_back(s[pos]);
            auto key = make_pair(pos + 1, st2);
            if(best.find(key) == best.end() || best[key] > initial) {
                best[key] = initial;
                q.push({pos + 1, st2, initial});
            }
        }
        else {
            char needed = tolower(s[pos]);
            if(st.empty()) {
                vector<char> st2;
                auto key = make_pair(pos + 1, st2);
                if(best.find(key) == best.end() || best[key] > initial + 1) {
                    best[key] = initial + 1;
                    q.push({pos + 1, st2, initial + 1});
                }
            } else if(st.back() == needed) {
                vector<char> st2 = st;
                st2.pop_back();
                auto key = make_pair(pos + 1, st2);
                if(best.find(key) == best.end() || best[key] > initial) {
                    best[key] = initial;
                    q.push({pos + 1, st2, initial});
                }
            }
        }
    }
    
    cout << result << endl;
    
    return 0;
}
