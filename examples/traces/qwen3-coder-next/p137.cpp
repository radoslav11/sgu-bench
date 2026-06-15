#include <iostream>
using namespace std;

int main() {
    int N, K;
    cin >> N >> K;

    for (int i = 1; i <= N; i++) {
        long long term1 = (long long)K * i;
        long long term2 = (long long)K * (i - 1);
        int a = term1 / N - term2 / N;
        if (i > 1) {
            cout << " ";
        }
        cout << a;
    }
    cout << endl;

    return 0;
}
