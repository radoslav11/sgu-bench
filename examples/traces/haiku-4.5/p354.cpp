#include<bits/stdc++.h>
using namespace std;

int n;
int top_mat[605][605], left_mat[605][605];
int result[605][605];
bool used[360005];

bool isValid(int i, int j, int val) {
    // Check left constraint
    int left_count = 0;
    for(int k = 0; k < j; k++) {
        if(result[i][k] > val) left_count++;
    }
    if(left_count != left_mat[i][j]) return false;
    
    // For top constraint, check what we can still place above
    int top_count = 0;
    for(int k = 0; k < i; k++) {
        if(result[k][j] > val) top_count++;
    }
    
    // Count how many values larger than val could still go above
    int can_place_above = 0;
    for(int v = val + 1; v <= n*n; v++) {
        if(!used[v]) can_place_above++;
    }
    
    // We need exactly top_mat[i][j] values greater than val above us
    // Currently have top_count, need top_mat[i][j]
    // Can still place (i - 1 - top_count) more values above us
    if(top_count > top_mat[i][j]) return false;
    if(top_count + (i - 1 - top_count) < top_mat[i][j]) return false;
    
    return true;
}

bool solve(int pos) {
    if(pos == n*n) return true;
    
    int i = pos / n;
    int j = pos % n;
    
    for(int val = 1; val <= n*n; val++) {
        if(used[val]) continue;
        
        result[i][j] = val;
        if(isValid(i, j, val)) {
            used[val] = true;
            if(solve(pos + 1)) return true;
            used[val] = false;
        }
    }
    result[i][j] = 0;
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> top_mat[i][j];
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> left_mat[i][j];
        }
    }
    
    if(solve(0)) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                cout << result[i][j];
                if(j < n-1) cout << " ";
            }
            cout << "\n";
        }
    } else {
        cout << "0\n";
    }
    
    return 0;
}
