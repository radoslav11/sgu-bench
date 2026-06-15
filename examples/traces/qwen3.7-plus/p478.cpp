#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int a, b;
    if (!(cin >> a >> b)) return 0;
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<int> c(n);
    for (int i = 0; i < n; ++i) {
        cin >> c[i];
    }
    
    int current_b = 0;
    int current_g = b;
    
    for (int i = 0; i < n; ++i) {
        int ci = c[i];
        
        // The number of girls remaining cannot exceed the previous number of girls,
        // and the number of boys cannot decrease. Thus, current_g - next_g >= 0 
        // and next_b >= current_b => ci - next_g >= current_b => next_g <= ci - current_b.
        int upper_g = min(current_g, ci - current_b);
        
        // The number of boys cannot exceed 'a'. Thus, next_b <= a => ci - next_g <= a => next_g >= ci - a.
        // Also, the number of girls cannot be negative.
        int lower_g = max(0, ci - a);
        
        if (lower_g > upper_g) {
            cout << "ERROR\n";
            return 0;
        }
        
        // Greedily choose the maximum possible number of girls to remain.
        // This minimizes the number of boys, which leaves the maximum possible room 
        // for boys to join in the future while keeping the upper bound on girls as loose as possible.
        int next_g = upper_g;
        int next_b = ci - next_g;
        
        int x = next_b - current_b;
        int y = current_g - next_g;
        
        cout << x << " " << y << "\n";
        
        current_b = next_b;
        current_g = next_g;
    }
    
    return 0;
}
