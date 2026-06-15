#include <cstdio>

const int MAXN = 65537;

int a[MAXN], tmp[MAXN];

long long merge_count(int l, int r) {
    if (r - l <= 1) return 0;

    int m = (l + r) / 2;
    long long inv = merge_count(l, m) + merge_count(m, r);

    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
            inv += m - i;
        }
    }

    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];

    for (int p = l; p < r; ++p) a[p] = tmp[p];

    return inv;
}

int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }

    printf("%lld\n", merge_count(0, n));

    return 0;
}
