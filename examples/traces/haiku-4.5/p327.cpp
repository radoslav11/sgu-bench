#include<bits/stdc++.h>
using namespace std;

int N;
vector<string> words;

bool isPalindrome(const string& s) {
    string rev = s;
    reverse(rev.begin(), rev.end());
    return s == rev;
}

bool containsWord(const string& s, const string& word) {
    return s.find(word) != string::npos;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    words.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> words[i];
    }
    
    // BFS approach
    queue<pair<string, int>> q;
    map<int, string> best;
    
    for (int i = 0; i < N; i++) {
        q.push({words[i], 1 << i});
        best[1 << i] = words[i];
    }
    
    string answer;
    
    while (!q.empty()) {
        auto [current, mask] = q.front();
        q.pop();
        
        if (mask == (1 << N) - 1 && isPalindrome(current)) {
            if (answer.empty() || current.length() < answer.length()) {
                answer = current;
            }
        }
        
        for (int i = 0; i < N; i++) {
            if (!(mask & (1 << i))) {
                string next = current + words[i];
                int new_mask = mask | (1 << i);
                if (!best.count(new_mask) || best[new_mask].length() > next.length()) {
                    best[new_mask] = next;
                    q.push({next, new_mask});
                }
            }
        }
    }
    
    cout << answer << endl;
    
    return 0;
}
