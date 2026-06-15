#include <iostream>
using namespace std;

int main() {
    long long n;
    cin >> n;
    
    long long zones = 1 + n * (n + 1) / 2;
    cout << zones << endl;
    
    return 0;
}
