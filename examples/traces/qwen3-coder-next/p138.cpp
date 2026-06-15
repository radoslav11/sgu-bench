#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> cnt(n);
    int total = 0;
    for (int i = 0; i < n; i++) {
        cin >> cnt[i];
        total += cnt[i];
    }
    int g = total / 2;
    cout << g << "\n";
    
    vector<int> rem = cnt;
    
    // Start with the player who has the most games
    int current = max_element(rem.begin(), rem.end()) - rem.begin();
    
    for (int i = 0; i < g; i++) {
        // Find the opponent: player with the highest remaining games, excluding current
        int opponent = -1;
        for (int j = 0; j < n; j++) {
            if (j == current) continue;
            if (rem[j] > 0) {
                if (opponent == -1 || rem[j] > rem[opponent]) {
                    opponent = j;
                }
            }
        }
        
        // If no opponent found (shouldn't happen), find any player with games left
        if (opponent == -1) {
            for (int j = 0; j < n; j++) {
                if (rem[j] > 0) {
                    opponent = j;
                    break;
                }
            }
        }
        
        // Decide the winner of this game
        if (i == 0 || rem[current] >= rem[opponent]) {
            // Current wins
            cout << current + 1 << " " << opponent + 1 << "\n";
            rem[current]--;
            rem[opponent]--;
        } else {
            // Opponent wins
            cout << opponent + 1 << " " << current + 1 << "\n";
            int temp = current;
            current = opponent;
            rem[temp]--;
            rem[current]--;
        }
    }
    
    return 0;
}
