#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int N, M;
    std::cin >> N >> M;
    
    std::vector<int> scores(M);
    for (int i = 0; i < M; i++) {
        std::cin >> scores[i];
    }
    
    std::vector<std::pair<std::string, std::string>> accepted;
    
    for (int faculty_idx = 0; faculty_idx < N; faculty_idx++) {
        // Read faculty name (may contain spaces)
        std::string faculty_name;
        std::cin.ignore(); // Ignore newline from previous input
        std::getline(std::cin, faculty_name);
        
        int K;
        std::cin >> K;
        std::vector<int> required_subjects(K);
        for (int i = 0; i < K; i++) {
            std::cin >> required_subjects[i];
        }
        
        std::vector<std::vector<int>> thresholds;
        for (int i = 0; i < K; i++) {
            int x, y, z;
            std::cin >> x >> y >> z;
            thresholds.push_back({x, y, z});
        }
        
        int S;
        std::cin >> S;
        
        for (int spec_idx = 0; spec_idx < S; spec_idx++) {
            std::string spec_name;
            std::cin.ignore(); // Ignore newline
            std::getline(std::cin, spec_name);
            
            int required_sum;
            std::cin >> required_sum;
            
            // Calculate Peter's sum of grades
            int total_grade = 0;
            for (int i = 0; i < K; i++) {
                int subject_num = required_subjects[i]; // 1-indexed
                int score = scores[subject_num - 1]; // convert to 0-indexed
                
                int x = thresholds[i][0];
                int y = thresholds[i][1];
                int z = thresholds[i][2];
                
                int grade;
                if (score < x) {
                    grade = 2;
                } else if (score < y) {
                    grade = 3;
                } else if (score < z) {
                    grade = 4;
                } else {
                    grade = 5;
                }
                
                total_grade += grade;
            }
            
            if (total_grade >= required_sum) {
                accepted.push_back({faculty_name, spec_name});
            }
        }
    }
    
    if (accepted.empty()) {
        std::cout << "Army" << std::endl;
    } else {
        for (const auto& p : accepted) {
            std::cout << p.first << " " << p.second << std::endl;
        }
    }
    
    return 0;
}
