#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, x;
    cin >> n >> x;
    vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }

    // dp_left[i][s]: can we form sum s using the first i coins (indices 0 to i-1)
    vector<vector<bool>> dp_left(n+1, vector<bool>(x+1, false));
    dp_left[0][0] = true;

    for (int i = 0; i < n; i++) {
        for (int s = 0; s <= x; s++) {
            dp_left[i+1][s] = dp_left[i][s];
            if (s >= coins[i] && dp_left[i][s - coins[i]]) {
                dp_left[i+1][s] = true;
            }
        }
    }

    // dp_right[i][s]: can we form sum s using coins from index i to n-1
    vector<vector<bool>> dp_right(n+1, vector<bool>(x+1, false));
    dp_right[n][0] = true;

    for (int i = n-1; i >= 0; i--) {
        for (int s = 0; s <= x; s++) {
            dp_right[i][s] = dp_right[i+1][s];
            if (s >= coins[i] && dp_right[i+1][s - coins[i]]) {
                dp_right[i][s] = true;
            }
        }
    }

    vector<int> necessary;
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int s = 0; s <= x; s++) {
            if (dp_left[i][s] && dp_right[i+1][x - s]) {
                found = true;
                break;
            }
        }
        if (!found) {
            necessary.push_back(coins[i]);
        }
    }

    cout << necessary.size() << endl;
    for (int i = 0; i < necessary.size(); i++) {
        if (i > 0) cout << " ";
        cout << necessary[i];
    }
    cout << endl;

    return 0;
}
