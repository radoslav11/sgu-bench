#include<bits/stdc++.h>
using namespace std;

int main(){
    long long a, b, c, x1, x2, y1, y2;
    cin >> a >> b >> c;
    cin >> x1 >> x2;
    cin >> y1 >> y2;
    
    long long count = 0;
    
    // Special case: both a and b are 0
    if(a == 0 && b == 0){
        if(c == 0){
            // All points in the rectangle are solutions
            count = (x2 - x1 + 1) * (y2 - y1 + 1);
        }
        // else c != 0, no solutions
    }
    // Special case: a = 0, b != 0
    else if(a == 0){
        // by + c = 0, so y = -c/b
        if(c % b == 0){
            long long y = -c / b;
            if(y >= y1 && y <= y2){
                count = x2 - x1 + 1;
            }
        }
    }
    // Special case: b = 0, a != 0
    else if(b == 0){
        // ax + c = 0, so x = -c/a
        if(c % a == 0){
            long long x = -c / a;
            if(x >= x1 && x <= x2){
                count = y2 - y1 + 1;
            }
        }
    }
    // General case: a != 0 and b != 0
    else {
        for(long long x = x1; x <= x2; x++){
            long long numerator = -a * x - c;
            if(numerator % b == 0){
                long long y = numerator / b;
                if(y >= y1 && y <= y2){
                    count++;
                }
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
