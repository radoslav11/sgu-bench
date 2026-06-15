#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K, N;
    if (!(cin >> K >> N)) return 0;

    vector<int> numbers(N);
    for (int i = 0; i < N; ++i) {
        cin >> numbers[i];
    }

    sort(numbers.begin(), numbers.end());

    int pages = 2;
    int lines = 0;
    int current_first_digit = -1;

    for (int i = 0; i < N; ++i) {
        int first_digit = numbers[i] / 1000;
        if (first_digit != current_first_digit) {
            if (lines > 0) {
                pages++;
                lines = 0;
            }
            current_first_digit = first_digit;
        }
        lines++;
        if (lines == K) {
            pages++;
            lines = 0;
        }
    }

    if (lines > 0) {
        pages++;
    }

    cout << pages << "\n";

    return 0;
}
