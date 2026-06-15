#include<bits/stdc++.h>
using namespace std;

int N;
int B[3][3];
int A[3][3];
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

bool isValid(int pos) {
    // Check if assignment up to position pos is valid
    int i = pos / N;
    int j = pos % N;
    
    // Check constraints for cell (i,j)
    int count = 0;
    for(int d = 0; d < 4; d++) {
        int ni = i + dx[d];
        int nj = j + dy[d];
        if(ni >= 0 && ni < N && nj >= 0 && nj < N) {
            if(A[ni][nj] > A[i][j]) {
                count++;
            }
        }
    }
    
    if(count != B[i][j]) return false;
    return true;
}

bool checkFinal() {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            int count = 0;
            for(int d = 0; d < 4; d++) {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if(ni >= 0 && ni < N && nj >= 0 && nj < N) {
                    if(A[ni][nj] > A[i][j]) {
                        count++;
                    }
                }
            }
            if(count != B[i][j]) return false;
        }
    }
    return true;
}

bool solve(int pos) {
    if(pos == N * N) {
        return checkFinal();
    }
    
    int i = pos / N;
    int j = pos % N;
    
    for(int val = 0; val <= 9; val++) {
        A[i][j] = val;
        
        // Check if this partial assignment can be valid
        bool canContinue = true;
        
        // Check constraint for current cell
        int count = 0;
        for(int d = 0; d < 4; d++) {
            int ni = i + dx[d];
            int nj = j + dy[d];
            if(ni >= 0 && ni < N && nj >= 0 && nj < N) {
                if(A[ni][nj] > A[i][j]) {
                    count++;
                }
            }
        }
        
        if(count > B[i][j]) {
            canContinue = false;
        }
        
        if(canContinue && solve(pos + 1)) {
            return true;
        }
    }
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> B[i][j];
        }
    }
    
    if(solve(0)) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(j > 0) cout << " ";
                cout << A[i][j];
            }
            cout << "\n";
        }
    } else {
        cout << "NO SOLUTION\n";
    }
    
    return 0;
}
