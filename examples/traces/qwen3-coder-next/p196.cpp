#include <iostream>
#include <vector>

int main() {
    int N, M;
    std::cin >> N >> M;
    
    std::vector<int> degree(N + 1, 0);
    
    for (int i = 0; i < M; ++i) {
        int u, v;
        std::cin >> u >> v;
        degree[u]++;
        degree[v]++;
    }
    
    long long sum = 0;
    for (int i = 1; i <= N; ++i) {
        long long d = degree[i];
        sum += d * d;
    }
    
    std::cout << sum << std::endl;
    
    return 0;
}
