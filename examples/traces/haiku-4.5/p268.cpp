#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    string S1, S2;
    cin >> N >> S1 >> S2;
    
    auto isAlmostPermutative = [N](const string& s) -> bool {
        if((int)s.length() != N + 1) return false;
        for(int i = 0; i < s.length(); i++) {
            set<char> chars;
            for(int j = 0; j < s.length(); j++) {
                if(i != j) chars.insert(s[j]);
            }
            if((int)chars.size() == N) {
                bool valid = true;
                for(int j = 0; j < N; j++) {
                    if(chars.find('A' + j) == chars.end()) {
                        valid = false;
                        break;
                    }
                }
                if(valid) return true;
            }
        }
        return false;
    };
    
    auto isHyperAlmostPermutative = [N, &isAlmostPermutative](const string& s) -> bool {
        for(int i = 0; i + N + 1 <= (int)s.length(); i++) {
            if(!isAlmostPermutative(s.substr(i, N + 1))) return false;
        }
        return true;
    };
    
    string best = S1 + S2;
    
    for(int overlap = 1; overlap < N; overlap++) {
        string candidate = S1 + S2.substr(overlap);
        if(isHyperAlmostPermutative(candidate) && candidate.length() < best.length()) {
            best = candidate;
        }
        
        candidate = S2 + S1.substr(overlap);
        if(isHyperAlmostPermutative(candidate) && candidate.length() < best.length()) {
            best = candidate;
        }
    }
    
    cout << best << endl;
    return 0;
}
