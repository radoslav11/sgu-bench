#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n, r;
    std::cin >> n >> r;
    std::vector<int> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int full_tables = 0;
    std::vector<int> remainders;
    for (int i = 0; i < n; i++) {
        full_tables += a[i] / r;
        int rem = a[i] % r;
        if (rem > 0) {
            remainders.push_back(rem);
        }
    }

    std::sort(remainders.rbegin(), remainders.rend());

    int tables = full_tables;
    int left = 0;
    int right = remainders.size() - 1;
    while (left <= right) {
        if (left == right) {
            tables++;
            left++;
        } else if (remainders[left] + remainders[right] <= r) {
            tables++;
            left++;
            right--;
        } else {
            tables++;
            left++;
        }
    }

    std::cout << tables << std::endl;

    return 0;
}
