#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N, T;
    cin >> N >> T;
    
    vector<int> L(N);
    for (int i = 0; i < N; i++) {
        cin >> L[i];
    }
    
    vector<int> V(N);
    for (int i = 0; i < N; i++) {
        cin >> V[i];
    }
    
    vector<int> count(N, 0);
    
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (V[i] * V[j] < 0) {
                int A0 = L[j] - L[i];
                int B = V[i] - V[j];
                
                for (int k = -2; k <= 10; k++) {
                    long long A = (long long)A0 + 1000LL * k;
                    if (B > 0) {
                        if (A >= 0 && A <= (long long)B * T) {
                            count[i]++;
                            count[j]++;
                        }
                    } else {
                        if (A <= 0 && A >= (long long)B * T) {
                            count[i]++;
                            count[j]++;
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < N; i++) {
        if (i > 0) cout << " ";
        cout << count[i];
    }
    cout << endl;
    
    return 0;
}
