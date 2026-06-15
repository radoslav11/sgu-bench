#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    deque<string> pile;
    
    // Read initial books
    for (int i = 0; i < N; i++) {
        string book;
        cin >> book;
        pile.push_back(book);
    }
    
    // Process operations
    for (int i = 0; i < M; i++) {
        string operation;
        cin >> operation;
        
        if (operation.substr(0, 3) == "ADD") {
            // Extract book name from ADD(S)
            int start = operation.find('(') + 1;
            int end = operation.find(')');
            string bookName = operation.substr(start, end - start);
            pile.push_front(bookName);
        } 
        else if (operation == "ROTATE") {
            // Rotate top K books (or all if less than K)
            int rotateCount = min(K, (int)pile.size());
            if (rotateCount > 0) {
                // Move the first element to position rotateCount-1
                string topBook = pile.front();
                pile.pop_front();
                pile.insert(pile.begin() + rotateCount - 1, topBook);
            }
        }
    }
    
    // Output the pile from top to bottom
    for (const auto& book : pile) {
        cout << book << "\n";
    }
    
    return 0;
}
