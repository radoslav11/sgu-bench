#include <iostream>
#include <string>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int r, y, g, br, bl, pi, k;
    if (!(cin >> r >> y >> g >> br >> bl >> pi >> k)) return 0;
    
    string state;
    cin >> state;
    
    int c = y * 2 + g * 3 + br * 4 + bl * 5 + pi * 6 + k * 7;
    
    int max_c = 0;
    if (k) max_c = 7;
    else if (pi) max_c = 6;
    else if (bl) max_c = 5;
    else if (br) max_c = 4;
    else if (g) max_c = 3;
    else if (y) max_c = 2;
    
    int ans = 0;
    if (r > 0) {
        if (state == "RED") {
            ans = 7 + r * 8 + c;
        } else {
            ans = r * 8 + c;
        }
    } else {
        if (state == "RED") {
            ans = max_c + c;
        } else {
            ans = c;
        }
    }
    
    cout << ans << "\n";
    return 0;
}
