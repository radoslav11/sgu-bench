#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    if (cin >> n) {
        if (n % 2 == 0) {
            cout << "No solution\n";
        } else {
            vector<int> steps;
            while (n > 1) {
                if (n % 4 == 1) {
                    steps.push_back(1);
                    n = (n + 1) / 2;
                } else {
                    steps.push_back(2);
                    n = (n - 1) / 2;
                }
            }
            cout << steps.size() << "\n";
            for (int i = (int)steps.size() - 1; i >= 0; --i) {
                cout << steps[i] << (i == 0 ? "" : " ");
            }
            cout << "\n";
        }
    }
    return 0;
}
