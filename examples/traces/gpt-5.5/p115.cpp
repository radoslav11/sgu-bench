#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (M < 1 || M > 12 || N < 1 || N > daysInMonth[M]) {
        cout << "Impossible\n";
        return 0;
    }

    int dayOfYear = N;
    for (int i = 1; i < M; ++i) {
        dayOfYear += daysInMonth[i];
    }

    int weekDay = (dayOfYear - 1) % 7 + 1;
    cout << weekDay << '\n';

    return 0;
}
