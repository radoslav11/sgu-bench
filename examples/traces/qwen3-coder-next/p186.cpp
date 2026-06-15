#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    if (n == 1) {
        // Only one chain, no connections needed
        cout << 0 << endl;
        // Skip the chain lengths
        int x;
        cin >> x;
        return 0;
    }
    
    // Read the chain lengths (they're not needed for the answer)
    for (int i = 0; i < n; i++) {
        int li;
        cin >> li;
    }
    
    // For n chains, we need n-1 connections, each taking 1 minute
    cout << n - 1 << endl;
    
    return 0;
}
