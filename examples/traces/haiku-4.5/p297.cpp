#include <iostream>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    long long totalBill = 0;
    for (int i = 0; i < M; i++) {
        int bill;
        cin >> bill;
        totalBill += bill;
    }
    
    int X = totalBill % N;
    cout << X << endl;
    
    return 0;
}
