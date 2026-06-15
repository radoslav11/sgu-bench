#include <iostream>

using namespace std;

int a[100005];
int temp_arr[100005];
long long inversions = 0;

void merge_sort(int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(left, mid);
    merge_sort(mid + 1, right);
    
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) {
            temp_arr[k++] = a[i++];
        } else {
            temp_arr[k++] = a[j++];
            inversions += (mid - i + 1);
        }
    }
    while (i <= mid) {
        temp_arr[k++] = a[i++];
    }
    while (j <= right) {
        temp_arr[k++] = a[j++];
    }
    for (int p = left; p <= right; ++p) {
        a[p] = temp_arr[p];
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    if (cin >> n) {
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        merge_sort(0, n - 1);
        cout << inversions << "\n";
    }
    return 0;
}
