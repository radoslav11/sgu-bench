#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    int exponent;
    cin >> exponent;
    
    long long sum = 0;
    
    for (int i = 0; i < n; i++) {
        long long potential;
        cin >> potential;
        
        long long power = 1;
        for (int j = 0; j < exponent; j++) {
            power *= potential;
        }
        
        if (power > 0) {
            sum += power;
        }
    }
    
    cout << sum << endl;
    
    return 0;
}
