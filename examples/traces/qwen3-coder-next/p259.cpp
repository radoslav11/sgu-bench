#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;
    
    std::vector<int> t(n);
    std::vector<int> l(n);
    
    for (int i = 0; i < n; ++i) {
        std::cin >> t[i];
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> l[i];
    }
    
    std::vector<std::pair<int, int>> leaflets;
    for (int i = 0; i < n; ++i) {
        leaflets.push_back({t[i], l[i]});
    }
    
    // Sort by delivery time in decreasing order
    std::sort(leaflets.begin(), leaflets.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    
    int total_time = 0;
    int current_sum = 0;
    
    for (int i = 0; i < n; ++i) {
        current_sum += leaflets[i].first;
        total_time = std::max(total_time, current_sum + leaflets[i].second);
    }
    
    std::cout << total_time << std::endl;
    
    return 0;
}
