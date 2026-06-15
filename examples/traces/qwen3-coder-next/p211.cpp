#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> state(N, 0);

    for (int idx = 0; idx < M; idx++) {
        int i;
        cin >> i;

        // Find the first position j >= i where state[j] < 2
        int j = i;
        while (j < N && state[j] == 2) {
            j++;
        }

        // Output: number of changes = j - i + 1
        cout << (j - i + 1);
        
        // For positions i to j-1: set to 1
        for (int k = i; k < j; k++) {
            cout << " " << k << " " << 1;
            state[k] = 1;
        }
        
        // For position j: set to state[j] + 1
        cout << " " << j << " " << (state[j] + 1);
        state[j] = state[j] + 1;
        
        cout << "\n";
    }

    return 0;
}
