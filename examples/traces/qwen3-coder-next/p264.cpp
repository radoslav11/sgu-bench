#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <cctype>
#include <algorithm>

using namespace std;

inline char get_char() {
    static char buf[1 << 20];
    static int idx = 0;
    static int len = 0;
    if (idx == len) {
        len = fread(buf, 1, sizeof(buf), stdin);
        idx = 0;
        if (len == 0) return 0;
    }
    return buf[idx++];
}

inline void skip_spaces() {
    char c;
    while ((c = get_char()) != 0 && isspace(c)) {}
    if (c != 0) {
        // Put back the non-space character
        static char last_char = c;
        static bool has_last_char = false;
        has_last_char = true;
    }
}

inline string read_string() {
    string res;
    char c;
    while ((c = get_char()) != 0 && !isspace(c)) {
        res += c;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<string> men(N);
    vector<vector<int>> man_pref(N, vector<int>(N));
    
    unordered_map<string, int> man_id;
    unordered_map<string, int> woman_id;
    
    // Read men's preferences
    for (int i = 0; i < N; i++) {
        string name;
        cin >> name;
        men[i] = name;
        man_id[name] = i;
        for (int j = 0; j < N; j++) {
            string w_name;
            cin >> w_name;
            if (woman_id.find(w_name) == woman_id.end()) {
                woman_id[w_name] = woman_id.size();
            }
            man_pref[i][j] = woman_id[w_name];
        }
    }
    
    // Read women's preferences
    vector<string> women(N);
    vector<vector<int>> woman_pref(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        string name;
        cin >> name;
        women[i] = name;
        woman_id[name] = i;
        for (int j = 0; j < N; j++) {
            string m_name;
            cin >> m_name;
            woman_pref[i][j] = man_id[m_name];
        }
    }
    
    // For each woman, compute ranking: woman_rank[w][m] = position of man m in woman w's list
    vector<vector<int>> woman_rank(N, vector<int>(N));
    for (int w = 0; w < N; w++) {
        for (int pos = 0; pos < N; pos++) {
            int m = woman_pref[w][pos];
            woman_rank[w][m] = pos;
        }
    }
    
    // Gale-Shapley algorithm: men propose to women
    vector<int> man_partner(N, -1);  // man_partner[m] = woman index assigned to man m
    vector<int> woman_partner(N, -1); // woman_partner[w] = man index assigned to woman w
    
    vector<int> next_proposal(N, 0);  // next_proposal[m] = index in man_pref[m] of next woman to propose to
    queue<int> free_men;  // men who haven't been matched yet
    
    for (int m = 0; m < N; m++) {
        free_men.push(m);
    }
    
    while (!free_men.empty()) {
        int m = free_men.front();
        free_men.pop();
        
        // Man m proposes to next woman on his list
        if (next_proposal[m] >= N) {
            continue; // should not happen in valid input
        }
        
        int w = man_pref[m][next_proposal[m]];
        next_proposal[m]++;
        
        if (woman_partner[w] == -1) {
            // Woman w is free, accepts proposal
            woman_partner[w] = m;
            man_partner[m] = w;
        } else {
            // Woman w is engaged, compare current partner vs proposer
            int current_m = woman_partner[w];
            if (woman_rank[w][m] < woman_rank[w][current_m]) {
                // Woman prefers new proposer m over current partner
                woman_partner[w] = m;
                man_partner[m] = w;
                man_partner[current_m] = -1;
                free_men.push(current_m); // current partner becomes free
            } else {
                // Woman rejects proposer, man remains free
                free_men.push(m);
            }
        }
    }
    
    cout << "YES" << endl;
    for (int m = 0; m < N; m++) {
        cout << men[m] << " " << women[man_partner[m]] << endl;
    }
    
    return 0;
}
