#include <bits/stdc++.h>
using namespace std;

int median3(int a, int b, int c) {
    int arr[3] = {a, b, c};
    sort(arr, arr+3);
    return arr[1];
}

int main() {
    int medians[3];
    for (int i = 0; i < 3; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        medians[i] = median3(a, b, c);
    }
    cout << median3(medians[0], medians[1], medians[2]) << endl;
    return 0;
}
