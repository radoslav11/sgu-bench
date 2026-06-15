#include <iostream>
using namespace std;

int main() {
    long long N;
    cin >> N;
    
    long long count = N / 3 + (N + 1) / 3;
    
    cout << count << endl;
    
    return 0;
}
