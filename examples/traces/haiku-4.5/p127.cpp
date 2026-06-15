#include<bits/stdc++.h>
using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::sort;

int main(){
    int K, N;
    cin >> K >> N;
    
    vector<string> phones(N);
    for(int i = 0; i < N; i++){
        cin >> phones[i];
    }
    
    // Sort phone numbers
    sort(phones.begin(), phones.end());
    
    int pages = 2; // First 2 pages for directory info
    
    int i = 0;
    while(i < N){
        char firstDigit = phones[i][0];
        int count = 0;
        
        // Count all numbers with the same first digit
        while(i < N && phones[i][0] == firstDigit){
            count++;
            i++;
        }
        
        // Calculate pages needed for this group
        int pagesForGroup = (count + K - 1) / K; // Ceiling division
        pages += pagesForGroup;
    }
    
    cout << pages << endl;
    
    return 0;
}
