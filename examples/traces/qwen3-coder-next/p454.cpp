#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

const int MAX_SIZE = 6;

// Directions: horizontal (left) and vertical (up)
const int DIR_HORZ = 0;
const int DIR_VERT = 1;

int n, m;
vector<string> grid;
vector<vector<int>> cellId;  // -1 for black cells, >=0 for white cells
vector<vector<pair<int, int>>> runs;  // runs[cellId] = {horizontalRunId, verticalRunId}
vector<vector<int>> runCellsHorz;  // runCellsHorz[runId] = list of cellIds in horizontal run
vector<vector<int>> runCellsVert;  // runCellsVert[runId] = list of cellIds in vertical run
vector<int> horzClues, vertClues;
vector<int> solution;  // solution[cellId] = digit placed
int whiteCellCount;
int totalRunsHorz, totalRunsVert;

// Check if placing 'digit' at 'cell' violates any constraints
bool canPlace(int cell, int digit, const vector<int>& current) {
    // Get horizontal and vertical run IDs for this cell
    int horzRunId = runs[cell].first;
    int vertRunId = runs[cell].second;
    
    // Check horizontal run: no duplicate digits
    if (horzRunId != -1) {
        for (int otherCell : runCellsHorz[horzRunId]) {
            if (otherCell != cell && current[otherCell] == digit) {
                return false;
            }
        }
        // Check if sum can still match: count filled cells and remaining
        int sumSoFar = 0;
        int emptyCount = 0;
        for (int otherCell : runCellsHorz[horzRunId]) {
            if (current[otherCell] != -1) {
                sumSoFar += current[otherCell];
            } else {
                emptyCount++;
            }
        }
        // Check if we've overfilled or can't reach the target
        if (emptyCount == 0) {
            if (sumSoFar != horzClues[horzRunId]) return false;
        } else {
            // Minimum possible sum for remaining cells: 1+2+...+emptyCount
            // Maximum possible sum: (9-emptyCount+1)+...+9
            int minRemaining = emptyCount * (emptyCount + 1) / 2;
            int maxRemaining = emptyCount * (19 - emptyCount) / 2;
            if (sumSoFar + minRemaining > horzClues[horzRunId] || 
                sumSoFar + maxRemaining < horzClues[horzRunId]) {
                return false;
            }
        }
    }
    
    // Check vertical run: no duplicate digits
    if (vertRunId != -1) {
        for (int otherCell : runCellsVert[vertRunId]) {
            if (otherCell != cell && current[otherCell] == digit) {
                return false;
            }
        }
        // Check sum constraints for vertical run
        int sumSoFar = 0;
        int emptyCount = 0;
        for (int otherCell : runCellsVert[vertRunId]) {
            if (current[otherCell] != -1) {
                sumSoFar += current[otherCell];
            } else {
                emptyCount++;
            }
        }
        if (emptyCount == 0) {
            if (sumSoFar != vertClues[vertRunId]) return false;
        } else {
            int minRemaining = emptyCount * (emptyCount + 1) / 2;
            int maxRemaining = emptyCount * (19 - emptyCount) / 2;
            if (sumSoFar + minRemaining > vertClues[vertRunId] || 
                sumSoFar + maxRemaining < vertClues[vertRunId]) {
                return false;
            }
        }
    }
    
    return true;
}

bool backtrack(int cellIdx, vector<int>& current) {
    if (cellIdx == whiteCellCount) {
        // All cells filled - verify all run sums
        for (int i = 0; i < totalRunsHorz; i++) {
            int sum = 0;
            for (int cell : runCellsHorz[i]) {
                sum += current[cell];
            }
            if (sum != horzClues[i]) return false;
        }
        for (int i = 0; i < totalRunsVert; i++) {
            int sum = 0;
            for (int cell : runCellsVert[i]) {
                sum += current[cell];
            }
            if (sum != vertClues[i]) return false;
        }
        return true;
    }
    
    for (int digit = 1; digit <= 9; digit++) {
        current[cellIdx] = digit;
        if (canPlace(cellIdx, digit, current)) {
            if (backtrack(cellIdx + 1, current)) {
                return true;
            }
        }
    }
    
    current[cellIdx] = -1;
    return false;
}

int main() {
    // Read input
    cin >> n >> m;
    cin.ignore(); // ignore newline
    
    grid.resize(n);
    for (int i = 0; i < n; i++) {
        getline(cin, grid[i]);
    }
    
    // Parse the grid and assign IDs to white cells
    cellId.assign(n, vector<int>(m, -1));
    whiteCellCount = 0;
    
    for (int i = 0; i < n; i++) {
        // Split the line into m tokens
        vector<string> tokens;
        string token;
        stringstream ss(grid[i]);
        while (ss >> token) {
            tokens.push_back(token);
        }
        
        for (int j = 0; j < m && j < tokens.size(); j++) {
            string cell = tokens[j];
            if (cell == ".....") {
                cellId[i][j] = whiteCellCount++;
            }
        }
    }
    
    // Find horizontal runs and their clues
    runs.assign(whiteCellCount, {-1, -1});
    
    // Process horizontal runs
    horzClues.clear();
    totalRunsHorz = 0;
    
    for (int i = 0; i < n; i++) {
        int j = 0;
        while (j < m) {
            // Skip black cells
            if (cellId[i][j] == -1) {
                j++;
                continue;
            }
            
            // Start of a horizontal run
            vector<int> runCells;
            int k = j;
            while (k < m && cellId[i][k] != -1) {
                runCells.push_back(cellId[i][k]);
                k++;
            }
            
            if (!runCells.empty()) {
                // Check clue in the black cell to the left
                int clue = -1;
                if (j > 0) {
                    // Extract clue from grid line
                    vector<string> tokens;
                    string token;
                    stringstream ss(grid[i]);
                    while (ss >> token) {
                        tokens.push_back(token);
                    }
                    if (j < tokens.size()) {
                        string clueStr = tokens[j-1];
                        if (clueStr != "XXXXX" && clueStr.find('\\') != string::npos) {
                            string horzClueStr = clueStr.substr(clueStr.find('\\') + 1);
                            if (horzClueStr != "XX" && horzClueStr.length() == 2) {
                                clue = stoi(horzClueStr);
                            }
                        }
                    }
                }
                
                horzClues.push_back(clue);
                runCellsHorz.push_back(runCells);
                
                // Assign run ID to cells
                for (int cell : runCells) {
                    runs[cell].first = totalRunsHorz;
                }
                
                totalRunsHorz++;
                j = k;
            } else {
                j++;
            }
        }
    }
    
    // Process vertical runs
    vertClues.clear();
    totalRunsVert = 0;
    
    for (int j = 0; j < m; j++) {
        int i = 0;
        while (i < n) {
            // Skip black cells
            if (cellId[i][j] == -1) {
                i++;
                continue;
            }
            
            // Start of a vertical run
            vector<int> runCells;
            int k = i;
            while (k < n && cellId[k][j] != -1) {
                runCells.push_back(cellId[k][j]);
                k++;
            }
            
            if (!runCells.empty()) {
                // Check clue in the black cell above
                int clue = -1;
                if (i > 0) {
                    // Extract clue from grid line above
                    vector<string> tokens;
                    string token;
                    stringstream ss(grid[i-1]);
                    while (ss >> token) {
                        tokens.push_back(token);
                    }
                    if (j < tokens.size()) {
                        string clueStr = tokens[j];
                        if (clueStr != "XXXXX" && clueStr.find('\\') != string::npos) {
                            string vertClueStr = clueStr.substr(0, clueStr.find('\\'));
                            if (vertClueStr != "XX" && vertClueStr.length() == 2) {
                                clue = stoi(vertClueStr);
                            }
                        }
                    }
                }
                
                vertClues.push_back(clue);
                runCellsVert.push_back(runCells);
                
                // Assign run ID to cells
                for (int cell : runCells) {
                    runs[cell].second = totalRunsVert;
                }
                
                totalRunsVert++;
                i = k;
            } else {
                i++;
            }
        }
    }
    
    // Solve using backtracking
    solution.assign(whiteCellCount, -1);
    backtrack(0, solution);
    
    // Output the solution
    for (int i = 0; i < n; i++) {
        // Split line into tokens again
        vector<string> tokens;
        string token;
        stringstream ss(grid[i]);
        while (ss >> token) {
            tokens.push_back(token);
        }
        
        for (int j = 0; j < m; j++) {
            if (j > 0) cout << " ";
            
            if (cellId[i][j] == -1) {
                // Black cell
                cout << "_";
            } else {
                // White cell - print the solution digit
                cout << solution[cellId[i][j]];
            }
        }
        cout << "\n";
    }
    
    return 0;
}
