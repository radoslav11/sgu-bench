#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<int> counts(7);
    for (int i = 0; i < 7; i++) {
        std::cin >> counts[i];
    }
    
    std::string last_potted;
    std::cin >> last_potted;
    
    int r = counts[0];
    // Colours: yellow=2, green=3, brown=4, blue=5, pink=6, black=7
    std::vector<int> colour_values = {2, 3, 4, 5, 6, 7};
    int sum_colours = 2 + 3 + 4 + 5 + 6 + 7; // 27
    
    if (r > 0) {
        if (last_potted == "RED") {
            std::cout << 8 * r + 34 << std::endl;
        } else {
            std::cout << 8 * r + 27 << std::endl;
        }
    } else {
        // Find the first colour that is present (count == 1)
        int start_index = 0;
        for (int i = 0; i < 6; i++) {
            if (counts[i + 1] == 1) {
                start_index = i;
                break;
            }
        }
        
        // Sum from start_index to end
        int sum_remaining = 0;
        for (int i = start_index; i < 6; i++) {
            sum_remaining += colour_values[i];
        }
        
        if (last_potted == "RED") {
            std::cout << 7 + sum_remaining << std::endl;
        } else {
            std::cout << sum_remaining << std::endl;
        }
    }
    
    return 0;
}
