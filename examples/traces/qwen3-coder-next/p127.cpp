#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int K, N;
    std::cin >> K >> N;
    
    std::vector<std::string> numbers(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> numbers[i];
    }
    
    // Sort phone numbers
    std::sort(numbers.begin(), numbers.end());
    
    int record_pages = 0;
    int lines_on_current_page = 0;
    
    for (int i = 0; i < N; ++i) {
        // Check if this number has a different first digit than previous
        bool new_page_needed = false;
        
        if (i == 0) {
            // First number always needs a new page
            new_page_needed = true;
        } else {
            // Check if first digit changed
            if (numbers[i][0] != numbers[i-1][0]) {
                new_page_needed = true;
            }
        }
        
        // If current page is full, we also need a new page
        if (lines_on_current_page >= K) {
            new_page_needed = true;
        }
        
        if (new_page_needed) {
            record_pages++;
            lines_on_current_page = 0;
        }
        
        // Add current number to the page
        lines_on_current_page++;
    }
    
    // Total pages = 2 (title and instructions) + record pages
    int total_pages = 2 + record_pages;
    
    std::cout << total_pages << std::endl;
    
    return 0;
}
