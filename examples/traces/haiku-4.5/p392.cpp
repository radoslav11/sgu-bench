#include<bits/stdc++.h>
using namespace std;

int R, C;
vector<string> arrows;
vector<string> letters;

pair<int,int> getNext(int r, int c) {
    char dir = arrows[r][c];
    if (dir == '<') return {r, c-1};
    if (dir == '>') return {r, c+1};
    if (dir == '^') return {r-1, c};
    if (dir == 'v') return {r+1, c};
    return {-1, -1};
}

bool isValid(int r, int c) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

bool canMatch(int sr, int sc, const string& word) {
    long long pos = 0;
    int r = sr, c = sc;
    map<pair<int,int>, long long> visited;
    
    while (pos < (long long)word.length()) {
        if (!isValid(r, c)) return false;
        
        if (letters[r][c] != word[pos]) return false;
        pos++;
        
        if (pos >= (long long)word.length()) return true;
        
        auto cell = make_pair(r, c);
        if (visited.count(cell)) {
            // We've been here before - cycle detected
            long long cycleStart = visited[cell];
            long long cycleLen = pos - cycleStart;
            
            if (cycleLen == 0) {
                // Dead end, can't continue
                auto next = getNext(r, c);
                if (!isValid(next.first, next.second)) return false;
                r = next.first;
                c = next.second;
            } else {
                long long remaining = word.length() - pos;
                long long cyclePos = remaining % cycleLen;
                
                // We need to match cyclePos more characters from current position
                for (long long i = 0; i < cyclePos; i++) {
                    auto next = getNext(r, c);
                    if (!isValid(next.first, next.second)) return false;
                    r = next.first;
                    c = next.second;
                    if (letters[r][c] != word[pos + i]) return false;
                }
                return true;
            }
        } else {
            visited[cell] = pos;
        }
        
        auto next = getNext(r, c);
        r = next.first;
        c = next.second;
    }
    
    return true;
}

string decompress(string s) {
    string result = "";
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '(') {
            int j = i + 1;
            while (s[j] != ')') j++;
            string frag = s.substr(i+1, j-i-1);
            int k = 0;
            int kstart = j + 1;
            while (kstart < (int)s.length() && isdigit(s[kstart])) {
                k = k * 10 + (s[kstart] - '0');
                kstart++;
            }
            for (int rep = 0; rep < k; rep++) {
                result += frag;
            }
            i = kstart - 1;
        } else {
            result += s[i];
        }
    }
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> R >> C;
    arrows.resize(R);
    letters.resize(R);
    
    for (int i = 0; i < R; i++) cin >> arrows[i];
    for (int i = 0; i < R; i++) cin >> letters[i];
    
    int Q;
    cin >> Q;
    
    while (Q--) {
        string query;
        cin >> query;
        string word = decompress(query);
        
        bool found = false;
        for (int r = 0; r < R && !found; r++) {
            for (int c = 0; c < C && !found; c++) {
                if (canMatch(r, c, word)) {
                    cout << "YES (" << (r+1) << "," << (c+1) << ")\n";
                    found = true;
                }
            }
        }
        if (!found) {
            cout << "NO\n";
        }
    }
    
    return 0;
}
