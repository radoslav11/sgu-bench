#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    vector<int> perm(n);
    for(int i = 0; i < n; i++) perm[i] = i + 1;
    
    int totalWinners = 0;
    int derangementCount = 0;
    
    do {
        // Check if it's a derangement (no fixed points)
        bool isDerangement = true;
        for(int i = 0; i < n; i++){
            if(perm[i] == i + 1){
                isDerangement = false;
                break;
            }
        }
        
        if(isDerangement){
            derangementCount++;
            // All judges in a derangement can potentially win
            totalWinners += n;
        }
    } while(next_permutation(perm.begin(), perm.end()));
    
    double answer = (double)totalWinners / derangementCount;
    cout << fixed << setprecision(10) << answer << endl;
    
    return 0;
}
