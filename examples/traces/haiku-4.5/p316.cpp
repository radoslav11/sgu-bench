#include <iostream>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<int> hp(N + 1, 100);      // hit points for tanks 1 to N
    vector<int> score(N + 1, 0);     // score for tanks 1 to N
    
    for (int i = 0; i < M; i++) {
        int shooter, target;
        cin >> shooter >> target;
        
        // Check if target had HP > 0 before the shot
        if (hp[target] > 0) {
            // Shooter gets 3 points
            score[shooter] += 3;
        }
        
        // Reduce target's HP by 8
        hp[target] -= 8;
        
        // HP cannot go below 0
        if (hp[target] < 0) {
            hp[target] = 0;
        }
    }
    
    // Add bonus points for tanks with HP > 0
    for (int i = 1; i <= N; i++) {
        if (hp[i] > 0) {
            score[i] += hp[i] / 2;
        }
    }
    
    // Output results
    for (int i = 1; i <= N; i++) {
        cout << hp[i] << " " << score[i] << "\n";
    }
    
    return 0;
}
