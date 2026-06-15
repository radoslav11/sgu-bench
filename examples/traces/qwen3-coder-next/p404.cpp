#include <iostream>
#include <string>
#include <vector>

int main() {
    int N, M;
    std::cin >> N >> M;
    std::cin.ignore(); // Ignore the newline after reading N and M

    std::vector<std::string> phrases(M);
    for (int i = 0; i < M; ++i) {
        std::getline(std::cin, phrases[i]);
    }

    // The phrases are indexed from 0 to M-1.
    // We start at index 0 for the first petal.
    // After N petals, we end up at index (N-1) % M.
    int result_index = (N - 1) % M;
    
    std::cout << phrases[result_index] << std::endl;

    return 0;
}
