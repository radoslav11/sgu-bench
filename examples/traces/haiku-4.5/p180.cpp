#include<bits/stdc++.h>
using namespace std;

long long mergeAndCount(vector<long long>& arr, int left, int mid, int right) {
    vector<long long> temp;
    int i = left;
    int j = mid + 1;
    long long invCount = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i]);
            i++;
        } else {
            temp.push_back(arr[j]);
            invCount += (mid - i + 1);
            j++;
        }
    }
    
    while (i <= mid) {
        temp.push_back(arr[i]);
        i++;
    }
    
    while (j <= right) {
        temp.push_back(arr[j]);
        j++;
    }
    
    for (int i = left, k = 0; i <= right; i++, k++) {
        arr[i] = temp[k];
    }
    
    return invCount;
}

long long mergeSortAndCount(vector<long long>& arr, int left, int right) {
    long long invCount = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;
        invCount += mergeSortAndCount(arr, left, mid);
        invCount += mergeSortAndCount(arr, mid + 1, right);
        invCount += mergeAndCount(arr, left, mid, right);
    }
    return invCount;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<long long> arr(n);
    
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    long long result = mergeSortAndCount(arr, 0, n - 1);
    cout << result << endl;
    
    return 0;
}
