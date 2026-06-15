#include <iostream>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    long long total = 0;
    for (int i = 0; i < M; i++) {
        int S;
        cin >> S;
        total += S;
    }
    cout << total % N << endl;
    return 0;
}
