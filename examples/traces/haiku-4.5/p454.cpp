#include<bits/stdc++.h>
using namespace std;

int n, m;
vector<string> grid;
vector<vector<int>> solution;
vector<pair<int,int>> whiteCells;
vector<vector<pair<int,int>>> runs;
vector<int> runClues;

bool isWhite(int r, int c) {
    return grid[r][c] == ".....";
}

void findRuns() {
    set<pair<vector<pair<int,int>>, int>> runSet;
    
    // Horizontal runs
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(grid[i][j].length() == 5 && grid[i][j][3] != 'X') {
                // Has horizontal clue
                int clue = stoi(grid[i][j].substr(3, 2));
                vector<pair<int,int>> run;
                int k = j + 1;
                while(k < m && isWhite(i, k)) {
                    run.push_back({i, k});
                    k++;
                }
                if(!run.empty()) {
                    runSet.insert({run, clue});
                }
            }
        }
    }
    
    // Vertical runs
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(grid[i][j].length() == 5 && grid[i][j][0] != 'X') {
                // Has vertical clue
                int clue = stoi(grid[i][j].substr(0, 2));
                vector<pair<int,int>> run;
                int k = i + 1;
                while(k < n && isWhite(k, j)) {
                    run.push_back({k, j});
                    k++;
                }
                if(!run.empty()) {
                    runSet.insert({run, clue});
                }
            }
        }
    }
    
    for(auto& p : runSet) {
        runs.push_back(p.first);
        runClues.push_back(p.second);
    }
}

bool isValid() {
    for(int i = 0; i < runs.size(); i++) {
        int sum = 0;
        set<int> used;
        bool complete = true;
        
        for(auto& cell : runs[i]) {
            int r = cell.first;
            int c = cell.second;
            if(solution[r][c] == 0) {
                complete = false;
                break;
            }
            int digit = solution[r][c];
            if(used.count(digit)) return false;
            used.insert(digit);
            sum += digit;
        }
        
        if(complete && sum != runClues[i]) return false;
        if(!complete && sum >= runClues[i]) return false;
    }
    return true;
}

bool solve(int idx) {
    if(idx == whiteCells.size()) {
        return isValid();
    }
    
    int r = whiteCells[idx].first;
    int c = whiteCells[idx].second;
    
    for(int digit = 1; digit <= 9; digit++) {
        solution[r][c] = digit;
        if(isValid()) {
            if(solve(idx + 1)) return true;
        }
    }
    
    solution[r][c] = 0;
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m;
    cin.ignore();
    
    grid.resize(n);
    solution.assign(n, vector<int>(m, 0));
    
    for(int i = 0; i < n; i++) {
        getline(cin, grid[i]);
        for(int j = 0; j < m; j++) {
            string cell = grid[i].substr(j*6, 5);
            if(cell == ".....") {
                whiteCells.push_back({i, j});
            }
        }
    }
    
    findRuns();
    solve(0);
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(j > 0) cout << " ";
            string cell = grid[i].substr(j*6, 5);
            if(cell == ".....") {
                cout << solution[i][j];
            } else {
                cout << "_";
            }
        }
        cout << "\n";
    }
    
    return 0;
}
