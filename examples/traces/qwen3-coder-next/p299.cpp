#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

int main() {
    int n;
    std::cin >> n;
    
    std::vector<int> segments(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> segments[i];
    }
    
    // Sort the segments in non-decreasing order
    std::sort(segments.begin(), segments.end());
    
    // Check for any three consecutive segments that can form a triangle
    // For three segments a <= b <= c, they form a triangle iff a + b > c
    // We only need to check consecutive triplets because if a <= b <= c <= d,
    // and a + b > d, then certainly b + c > d and a + c > d, but we want the 
    // most likely candidates which are the largest possible segments that satisfy
    // the condition. Checking consecutive triplets after sorting is sufficient
    // because if any triplet satisfies the triangle inequality, there must be
    // some triplet that does, and checking consecutive triplets is efficient.
    
    std::vector<int> result;
    bool found = false;
    
    // We'll check all possible triplets, but optimize by checking larger segments first
    // Actually, for correctness, we can check all triplets, but n is only up to 1000
    // So O(n^3) is too slow (10^9 operations worst-case)
    // Instead, after sorting, we can check for each c (largest side), 
    // if there exist a and b (with a <= b < c) such that a + b > c
    // Since the array is sorted, for a fixed c = segments[i], we can check 
    // if segments[i-2] + segments[i-1] > segments[i]
    // If this holds, then we have a triangle.
    
    // Why consecutive? Because if segments[j] + segments[k] > segments[i] for j < k < i,
    // then segments[k-1] + segments[k] >= segments[j] + segments[k] > segments[i] 
    // only if segments[k-1] >= segments[j], which is true since sorted.
    // Actually, the best chance for segments[j] + segments[k] > segments[i] is when 
    // j and k are as large as possible (i.e., j = i-2, k = i-1).
    
    for (int i = 2; i < n; ++i) {
        if (segments[i-2] + segments[i-1] > segments[i]) {
            result = {segments[i-2], segments[i-1], segments[i]};
            found = true;
            break;
        }
    }
    
    if (found) {
        std::cout << result[0] << " " << result[1] << " " << result[2] << std::endl;
    } else {
        std::cout << "0 0 0" << std::endl;
    }
    
    return 0;
}
