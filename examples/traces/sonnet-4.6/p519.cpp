#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> h(n, vector<int>(m));
    for(int i = 0; i < n; i++){
        string s;
        cin >> s;
        for(int j = 0; j < m; j++){
            h[i][j] = s[j] - '0';
        }
    }
    
    long long area = 0;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(h[i][j] > 0){
                area += 2; // top and bottom
            }
            // Internal horizontal (right neighbor)
            if(j+1 < m){
                area += abs(h[i][j] - h[i][j+1]);
            }
            // Internal vertical (down neighbor)
            if(i+1 < n){
                area += abs(h[i][j] - h[i+1][j]);
            }
            // Boundary
            if(i == 0) area += h[i][j];
            if(i == n-1) area += h[i][j];
            if(j == 0) area += h[i][j];
            if(j == m-1) area += h[i][j];
        }
    }
    
    cout << area << endl;
    return 0;
}
