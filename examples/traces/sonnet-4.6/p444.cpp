#include <bits/stdc++.h>
using namespace std;

int main() {
    long long x, y;
    cin >> x >> y;
    
    long long r = x % y;
    
    if (r == 0) {
        cout << "0 0" << endl;
        return 0;
    }
    
    map<long long, int> seen;
    int step = 0;
    
    while (true) {
        if (r == 0) {
            // finite
            cout << step << " " << 0 << endl;
            return 0;
        }
        if (seen.count(r)) {
            int start = seen[r];
            int length = step - start;
            cout << start << " " << length << endl;
            return 0;
        }
        seen[r] = step;
        r = (r * 10) % y;
        step++;
    }
    
    return 0;
}
