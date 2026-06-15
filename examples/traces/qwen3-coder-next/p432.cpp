#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// Helper function to check if a word (lowercase) is one of the target words for "not"
bool isCanMayMustShould(const string& word) {
    string w = word;
    transform(w.begin(), w.end(), w.begin(), ::tolower);
    return w == "can" || w == "may" || w == "must" || w == "should";
}

// Helper function to check if a word (lowercase) is "is" or "are"
bool isIsAre(const string& word) {
    string w = word;
    transform(w.begin(), w.end(), w.begin(), ::tolower);
    return w == "is" || w == "are";
}

// Helper function to check if a word is an article
bool isArticle(const string& word) {
    string w = word;
    transform(w.begin(), w.end(), w.begin(), ::tolower);
    return w == "a" || w == "the";
}

int main() {
    // Read entire input
    string text;
    string line;
    while (getline(cin, line)) {
        text += line + " ";
    }
    
    // Trim trailing space
    if (!text.empty() && text.back() == ' ') {
        text.pop_back();
    }
    
    // Split into sentences: split by ". " (dot followed by space or end)
    vector<string> sentences;
    size_t start = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '.') {
            // Extract sentence from start to i (inclusive)
            string sentence = text.substr(start, i - start + 1);
            sentences.push_back(sentence);
            // Next sentence starts after ". " (if exists)
            if (i + 1 < text.size() && text[i+1] == ' ') {
                start = i + 2;
                ++i; // skip the space
            } else {
                start = i + 1;
            }
        }
    }
    
    vector<string> results;
    
    // Process each sentence
    for (const string& sent : sentences) {
        // Split sentence into words (without the dot at the end)
        // Remove the trailing dot first
        string cleanSent = sent;
        if (!cleanSent.empty() && cleanSent.back() == '.') {
            cleanSent.pop_back();
        }
        
        // Split by space
        vector<string> words;
        istringstream iss(cleanSent);
        string word;
        while (iss >> word) {
            words.push_back(word);
        }
        
        // For each position, check if we can apply negation
        for (int i = 0; i < (int)words.size(); ++i) {
            // Case 1: Insert "not" after can/may/must/should
            if (isCanMayMustShould(words[i])) {
                // Build new sentence
                vector<string> newWords;
                for (int j = 0; j < i + 1; ++j) {
                    newWords.push_back(words[j]);
                }
                newWords.push_back("not");
                for (int j = i + 1; j < (int)words.size(); ++j) {
                    newWords.push_back(words[j]);
                }
                
                // Reconstruct sentence with dot
                string newSent = "";
                for (int j = 0; j < (int)newWords.size(); ++j) {
                    if (j > 0) newSent += " ";
                    newSent += newWords[j];
                }
                newSent += ".";
                results.push_back(newSent);
            }
            
            // Case 2: Insert "no" after is/are
            if (isIsAre(words[i])) {
                vector<string> newWords;
                for (int j = 0; j < i + 1; ++j) {
                    newWords.push_back(words[j]);
                }
                newWords.push_back("no");
                
                // Skip the next word if it is an article (a or the)
                int skipNext = 0;
                if (i + 1 < (int)words.size() && isArticle(words[i+1])) {
                    skipNext = 1;
                }
                
                for (int j = i + 1 + skipNext; j < (int)words.size(); ++j) {
                    newWords.push_back(words[j]);
                }
                
                string newSent = "";
                for (int j = 0; j < (int)newWords.size(); ++j) {
                    if (j > 0) newSent += " ";
                    newSent += newWords[j];
                }
                newSent += ".";
                results.push_back(newSent);
            }
        }
    }
    
    // Output
    cout << results.size() << endl;
    for (const string& res : results) {
        cout << res << endl;
    }
    
    return 0;
}
