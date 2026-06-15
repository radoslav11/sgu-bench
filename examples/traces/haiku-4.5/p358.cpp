#include <iostream>
#include <algorithm>
using namespace std;

int median_of_three(int a, int b, int c) {
    // Sort the three numbers and return the middle one
    int arr[3] = {a, b, c};
    sort(arr, arr + 3);
    return arr[1];
}

int main() {
    int a1, a2, a3;
    int b1, b2, b3;
    int c1, c2, c3;
    
    cin >> a1 >> a2 >> a3;
    cin >> b1 >> b2 >> b3;
    cin >> c1 >> c2 >> c3;
    
    int median1 = median_of_three(a1, a2, a3);
    int median2 = median_of_three(b1, b2, b3);
    int median3 = median_of_three(c1, c2, c3);
    
    int result = median_of_three(median1, median2, median3);
    
    cout << result << endl;
    
    return 0;
}
