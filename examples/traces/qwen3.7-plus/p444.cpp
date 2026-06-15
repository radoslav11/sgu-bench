#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int x, y;
    if (cin >> x >> y) {
        int r = x % y;
        vector<int> rem_pos(y, -1);
        int step = 0;
        
        while (true) {
            if (r == 0) {
                cout << step << " 0\n";
                break;
            }
            if (rem_pos[r] != -1) {
                cout << rem_pos[r] << " " << step - rem_pos[r] << "\n";
                break;
            }
            rem_pos[r] = step;
            step++;
            r = (r * 10) % y;
        }
    }
    
    return 0;
}
