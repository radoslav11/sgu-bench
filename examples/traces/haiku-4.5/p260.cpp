#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<vector<int>> affects(n); // affects[i] = cells affected when clicking cell i
    
    for(int i = 0; i < n; i++){
        int k;
        cin >> k;
        for(int j = 0; j < k; j++){
            int cell;
            cin >> cell;
            cell--; // 0-indexed
            affects[i].push_back(cell);
        }
    }
    
    string colors;
    cin >> colors;
    vector<int> initial(n);
    for(int i = 0; i < n; i++){
        initial[i] = colors[i] - '0';
    }
    
    // Try both target colors: all 0s and all 1s
    vector<int> best_solution;
    
    for(int target = 0; target <= 1; target++){
        // Create augmented matrix for the system
        // M[i][j] = 1 if clicking j affects cell i
        // M[i][n] = initial[i] XOR target (what we need to toggle)
        vector<bitset<201>> matrix(n);
        
        for(int i = 0; i < n; i++){
            // Column j represents clicking cell j
            for(int j = 0; j < n; j++){
                for(int affected : affects[j]){
                    if(affected == i){
                        matrix[i][j] = 1;
                        break;
                    }
                }
            }
            // Augmented column
            matrix[i][n] = (initial[i] != target) ? 1 : 0;
        }
        
        // Gaussian elimination over GF(2)
        vector<int> pivot_col(n, -1);
        int rank = 0;
        
        for(int col = 0; col < n && rank < n; col++){
            // Find pivot
            int pivot_row = -1;
            for(int row = rank; row < n; row++){
                if(matrix[row][col]){
                    pivot_row = row;
                    break;
                }
            }
            
            if(pivot_row == -1) continue;
            
            // Swap rows
            swap(matrix[rank], matrix[pivot_row]);
            pivot_col[rank] = col;
            
            // Eliminate
            for(int row = 0; row < n; row++){
                if(row != rank && matrix[row][col]){
                    matrix[row] ^= matrix[rank];
                }
            }
            rank++;
        }
        
        // Check if solvable
        bool solvable = true;
        for(int row = rank; row < n; row++){
            if(matrix[row][n]){
                solvable = false;
                break;
            }
        }
        
        if(solvable){
            vector<int> solution(n, 0);
            for(int row = 0; row < rank; row++){
                if(pivot_col[row] != -1){
                    solution[pivot_col[row]] = matrix[row][n];
                }
            }
            
            if(best_solution.empty() || __builtin_popcount(solution[0]) < best_solution.size()){
                best_solution = solution;
            }
        }
    }
    
    if(best_solution.empty()){
        cout << -1 << "\n";
    } else {
        vector<int> clicks;
        for(int i = 0; i < n; i++){
            if(best_solution[i]){
                clicks.push_back(i + 1); // 1-indexed
            }
        }
        cout << clicks.size() << "\n";
        for(int click : clicks){
            cout << click << " ";
        }
        if(!clicks.empty()) cout << "\n";
    }
    
    return 0;
}
