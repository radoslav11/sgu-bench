#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    if (cin >> N >> M) {
        if (M < 1 || M > 12) {
            cout << "Impossible\n";
            return 0;
        }
        
        int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        if (N < 1 || N > days_in_month[M]) {
            cout << "Impossible\n";
            return 0;
        }
        
        int day_of_year = N;
        for (int i = 1; i < M; ++i) {
            day_of_year += days_in_month[i];
        }
        
        int day_of_week = (day_of_year - 1) % 7 + 1;
        cout << day_of_week << "\n";
    }
    
    return 0;
}
