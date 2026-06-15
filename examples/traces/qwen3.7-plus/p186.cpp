#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<int> L(n);
    int cnt1 = 0;
    for (int i = 0; i < n; ++i) {
        cin >> L[i];
        if (L[i] == 1) {
            cnt1++;
        }
    }
    
    sort(L.begin(), L.end());
    
    int ops = 0;
    int i = 0;
    int chains_left = n;
    
    while (chains_left > 1) {
        if (cnt1 > 0) {
            // We have at least one chain of length 1.
            // Using it allows us to merge two other chains, reducing the total count by 2.
            if (chains_left >= 3) {
                chains_left -= 2;
                cnt1--;
                ops++;
            } else {
                // If only 2 chains are left (one of length 1 and one other), 
                // we just merge them, reducing the count by 1.
                chains_left = 1;
                cnt1--;
                ops++;
            }
        } else {
            // No chains of length 1 available. We must cut the smallest available chain.
            // This operation reduces the total chain count by 1.
            // If the chain had length 2, the remainder is length 1, which becomes available for the next steps.
            int rem = L[i] - 1;
            i++;
            chains_left--;
            ops++;
            if (rem == 1) {
                cnt1++;
            }
        }
    }
    
    cout << ops << "\n";
    return 0;
}
