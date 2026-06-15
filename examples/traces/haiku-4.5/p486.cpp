#include <iostream>
#include <string>
#include <map>

using namespace std;

int main() {
    string secret, guess;
    cin >> secret >> guess;
    
    int bulls = 0;
    int cows = 0;
    
    // Count bulls
    map<char, int> secret_remaining, guess_remaining;
    
    for (int i = 0; i < 4; i++) {
        if (secret[i] == guess[i]) {
            bulls++;
        } else {
            // Keep track of non-matching digits
            secret_remaining[secret[i]]++;
            guess_remaining[guess[i]]++;
        }
    }
    
    // Count cows from remaining digits
    for (auto& p : secret_remaining) {
        char digit = p.first;
        int secret_count = p.second;
        int guess_count = guess_remaining[digit];
        
        // The number of cows for this digit is the minimum of its counts
        cows += min(secret_count, guess_count);
    }
    
    cout << bulls << " " << cows << endl;
    
    return 0;
}
