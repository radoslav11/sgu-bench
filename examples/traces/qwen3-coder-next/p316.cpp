#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<int> hp(N + 1, 100);
    vector<int> score(N + 1, 0);
    
    for (int i = 0; i < M; i++) {
        int shooter, target;
        cin >> shooter >> target;
        
        // Check if target has HP before reducing
        if (hp[target] > 0) {
            score[shooter] += 3;
        }
        hp[target] -= 8;
    }
    
    // Add bonus points for active tanks
    for (int i = 1; i <= N; i++) {
        if (hp[i] > 0) {
            score[i] += hp[i] / 2;
        }
    }
    
    // Output results for each tank
    for (int i = 1; i <= N; i++) {
        cout << hp[i] << " " << score[i] << "\n";
    }
    
    return 0;
}
