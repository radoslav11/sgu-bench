#include <bits/stdc++.h>
using namespace std;

// Mapping from digit character to its position (0-indexed from least significant)
// and maximum quantity allowed
const vector<pair<char, int>> DIGIT_INFO = {
    {'_', 95, 2},
    {'.', 46, 3},
    {',', 44, 5},
    {'-', 45, 7},
    {'~', 126, 11},
    {'=', 61, 13},
    {'\'', 39, 17},
    {'^', 94, 19},
    {'"', 34, 23}
};

// Convert from digit character to index (0 = least significant = '_')
int charToIndex(char c) {
    for (int i = 0; i < (int)DIGIT_INFO.size(); i++) {
        if (DIGIT_INFO[i].first == c) return i;
    }
    return -1;
}

// Parse Diputs notation string to number
long long diputsToNum(const string& s) {
    if (s == "O") return 0;
    if (s.empty()) return -1;
    
    // Count occurrences of each digit (from most significant to least)
    vector<int> counts(9, 0);
    for (char c : s) {
        int idx = charToIndex(c);
        if (idx == -1) return -1;
        counts[idx]++;
    }
    
    // Check against max limits
    for (int i = 0; i < 9; i++) {
        if (counts[i] > DIGIT_INFO[i].second) return -1;
    }
    
    // Compute the number: digits are in order from most significant (index 8) to least (index 0)
    // So most significant digit is ", the " index 8 is '"', index 7 is '^', ..., index 0 is '_'
    // But the representation is written from most significant to least significant
    // Example: """^^~~-,..__ 
    // counts[8] = 3 (for "), counts[7] = 2 (for ^), etc.
    
    long long result = 0;
    long long base = 1;
    for (int i = 0; i < 9; i++) {
        result += (long long)counts[i] * base;
        base *= (DIGIT_INFO[i].second + 1);
    }
    return result;
}

// Convert number to Diputs notation string
string numToDiputs(long long n) {
    if (n == 0) return "O";
    
    vector<int> counts(9, 0);
    vector<long long> bases(10);
    bases[0] = 1;
    for (int i = 0; i < 9; i++) {
        bases[i+1] = bases[i] * (DIGIT_INFO[i].second + 1);
    }
    
    // Extract digits from most significant to least
    for (int i = 8; i >= 0; i--) {
        long long base = bases[i];
        counts[i] = n / base;
        n %= base;
    }
    
    // Build string: from most significant (index 8) to least significant (index 0)
    string res;
    for (int i = 8; i >= 0; i--) {
        for (int j = 0; j < counts[i]; j++) {
            res += DIGIT_INFO[i].first;
        }
    }
    return res;
}

// Maximum number representable in Diputs notation
long long getMaxDiputsNum() {
    long long maxVal = 0;
    long long base = 1;
    for (int i = 0; i < 9; i++) {
        maxVal += (long long)DIGIT_INFO[i].second * base;
        base *= (DIGIT_INFO[i].second + 1);
    }
    return maxVal;
}

// Try to parse Diputs representation starting at position pos
pair<long long, int> tryParseDiputs(const string& s, int pos) {
    if (pos >= s.size()) return {-1, 0};
    
    // Try to parse as "O" first
    if (s[pos] == 'O') {
        return {0, 1};
    }
    
    // Try to parse Diputs digits
    string candidate;
    int i = pos;
    while (i < s.size()) {
        char c = s[i];
        // Check if c is a valid Diputs digit
        if (c != '_' && c != '.' && c != ',' && c != '-' && c != '~' && 
            c != '=' && c != '\'' && c != '^' && c != '"') {
            break;
        }
        candidate += c;
        long long val = diputsToNum(candidate);
        if (val == -1 || val > getMaxDiputsNum()) {
            // Backtrack: remove last char and try
            candidate.pop_back();
            break;
        }
        i++;
    }
    
    if (candidate.empty()) return {-1, 0};
    
    long long val = diputsToNum(candidate);
    if (val == -1 || val > getMaxDiputsNum()) {
        // Try shorter prefix
        for (int j = candidate.size() - 1; j >= 0; j--) {
            candidate.pop_back();
            long long v = diputsToNum(candidate);
            if (v != -1 && v <= getMaxDiputsNum()) {
                return {v, (int)candidate.size()};
            }
        }
        return {-1, 0};
    }
    
    return {val, (int)candidate.size()};
}

// Try to parse decimal number starting at position pos (greedy, no leading zeros)
pair<long long, int> tryParseDecimal(const string& s, int pos) {
    if (pos >= s.size()) return {-1, 0};
    
    if (s[pos] == '0') {
        return {0, 1}; // Handle "0" specially
    }
    
    long long val = 0;
    int i = pos;
    while (i < s.size() && isdigit(s[i])) {
        val = val * 10 + (s[i] - '0');
        if (val > getMaxDiputsNum()) {
            // Need to split: return as many digits as possible that are <= max
            break;
        }
        i++;
    }
    
    // If we broke early due to exceeding max, go back one step
    if (i > pos && val > getMaxDiputsNum()) {
        i--;
        val = stoll(s.substr(pos, i - pos + 1));
    }
    
    if (i == pos) return {-1, 0};
    return {val, i - pos + 1};
}

// Main parsing function
vector<pair<long long, int>> parseInput(const string& s) {
    vector<pair<long long, int>> numbers; // (value, 0 for decimal, 1 for diputs)
    int n = s.size();
    int i = 0;
    
    while (i < n) {
        // Try to parse Diputs
        auto diputsRes = tryParseDiputs(s, i);
        // Try to parse decimal
        auto decimalRes = tryParseDecimal(s, i);
        
        bool isDiputs = diputsRes.second > 0;
        bool isDecimal = decimalRes.second > 0;
        
        // If both possible, prefer the longer one; if tie, prefer Diputs if it starts with digit char
        // According to problem: greedy parsing - take maximum number of characters
        if (!isDiputs && !isDecimal) {
            // Not a number, skip character
            i++;
            continue;
        }
        
        if (isDiputs && (!isDecimal || diputsRes.second >= decimalRes.second)) {
            numbers.push_back({diputsRes.first, 1}); // 1 means originally diputs
            i += diputsRes.second;
        } else if (isDecimal) {
            numbers.push_back({decimalRes.first, 0}); // 0 means originally decimal
            i += decimalRes.second;
        } else {
            i++;
        }
    }
    
    return numbers;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    string line;
    string input = "";
    
    while (getline(cin, line)) {
        input += line + "\n";
    }
    
    vector<pair<long long, int>> numbers = parseInput(input);
    
    // Convert all numbers: decimal to diputs, diputs to decimal
    vector<pair<long long, int>> converted;
    for (auto& p : numbers) {
        if (p.second == 0) { // was decimal, convert to diputs
            string diputsRep = numToDiputs(p.first);
            // Store original type 1 (diputs) since we converted it
            converted.push_back({p.first, 1});
        } else { // was diputs, convert to decimal
            converted.push_back({p.first, 0});
        }
    }
    
    // Extract only values to sort (for positions that are currently stored as (value, type))
    // But we need to sort preserving the notation type in each position
    vector<long long> values;
    vector<int> types;
    for (auto& p : converted) {
        values.push_back(p.first);
        types.push_back(p.second);
    }
    
    // Sort the values
    sort(values.begin(), values.end());
    
    // Reconstruct output: for each position, if original was decimal (types[i]==0) output decimal, else diputs
    vector<string> resultTokens;
    for (int i = 0; i < (int)values.size(); i++) {
        if (types[i] == 0) { // originally decimal, so output decimal
            if (values[i] == 0) {
                resultTokens.push_back("0");
            } else {
                resultTokens.push_back(to_string(values[i]));
            }
        } else { // originally diputs, so output diputs
            if (values[i] == 0) {
                resultTokens.push_back("O");
            } else {
                resultTokens.push_back(numToDiputs(values[i]));
            }
        }
    }
    
    // Now, we need to reconstruct the original input structure, but only the numbers changed
    // Instead, let's do a different approach: parse the input keeping positions of numbers and non-number parts
    
    // Alternative approach: tokenize the input, identifying number vs non-number tokens
    vector<tuple<string, int, long long>> tokens; // (original text, 0=non-num, 1=num, value)
    
    i = 0;
    int total = input.size();
    while (i < total) {
        // Try to parse Diputs
        auto diputsRes = tryParseDiputs(input, i);
        // Try to parse decimal
        auto decimalRes = tryParseDecimal(input, i);
        
        bool isDiputs = diputsRes.second > 0;
        bool isDecimal = decimalRes.second > 0;
        
        if (!isDiputs && !isDecimal) {
            int j = i;
            while (j < total) {
                string charStr = string(1, input[j]);
                auto dRes = tryParseDiputs(input, j);
                auto decRes = tryParseDecimal(input, j);
                if (dRes.second == 0 && decRes.second == 0) {
                    j++;
                } else {
                    break;
                }
            }
            tokens.push_back({input.substr(i, j - i), 0, 0});
            i = j;
        } else if (isDiputs && (!isDecimal || diputsRes.second >= decimalRes.second)) {
            tokens.push_back({input.substr(i, diputsRes.second), 1, diputsRes.first});
            i += diputsRes.second;
        } else if (isDecimal) {
            tokens.push_back({input.substr(i, decimalRes.second), 1, decimalRes.first});
            i += decimalRes.second;
        } else {
            i++;
        }
    }
    
    // Collect all numbers (type preserved: 1 for diputs, 0 for decimal)
    vector<tuple<long long, int>> nums;
    for (auto& tok : tokens) {
        if (get<1>(tok) == 1) {
            nums.push_back({get<2>(tok), -1}); // will set type later
        }
    }
    
    // Now, determine original notation for each number
    vector<int> numTypes;
    i = 0;
    for (auto& tok : tokens) {
        if (get<1>(tok) == 1) {
            // Determine if this token was originally diputs or decimal
            string s = get<0>(tok);
            if (s == "O") {
                numTypes.push_back(1); // diputs
            } else if (s[0] == 'O' && s.size() > 1) {
                // This should not happen due to greedy parsing, but just in case
                numTypes.push_back(1);
            } else if (s[0] == '0' && s.size() > 1) {
                // Decimal with leading zero
                numTypes.push_back(0);
            } else {
                // Check if it's all diputs digits
                bool allDiputs = true;
                for (char c : s) {
                    if (c != '_' && c != '.' && c != ',' && c != '-' && c != '~' && 
                        c != '=' && c != '\'' && c != '^' && c != '"') {
                        allDiputs = false;
                        break;
                    }
                }
                numTypes.push_back(allDiputs ? 1 : 0);
            }
        }
    }
    
    // Now convert numbers: if originally diputs (type=1), convert to decimal; if decimal (type=0), convert to diputs
    vector<long long> convertedNums;
    for (int j = 0; j < (int)nums.size(); j++) {
        long long val = get<0>(nums[j]);
        if (numTypes[j] == 0) {
            // was decimal, now convert to diputs (but we store as diputs token)
            if (val == 0) {
                convertedNums.push_back(-1); // marker for "O"
            } else {
                convertedNums.push_back(val);
            }
        } else {
            // was diputs, now convert to decimal
            convertedNums.push_back(val);
        }
    }
    
    // Sort the converted numbers
    vector<long long> sortedNums = convertedNums;
    sort(sortedNums.begin(), sortedNums.end());
    
    // Now reconstruct output: go through tokens and replace numbers with sorted ones
    vector<string> outputTokens;
    int numIdx = 0;
    for (auto& tok : tokens) {
        if (get<1>(tok) == 0) {
            outputTokens.push_back(get<0>(tok));
        } else {
            long long val = sortedNums[numIdx++];
            if (numTypes[numIdx-1] == 0) {
                // originally decimal, now should output decimal
                if (val == 0) {
                    outputTokens.push_back("0");
                } else {
                    outputTokens.push_back(to_string(val));
                }
            } else {
                // originally diputs, now should output diputs
                if (val == 0) {
                    outputTokens.push_back("O");
                } else {
                    outputTokens.push_back(numToDiputs(val));
                }
            }
        }
    }
    
    // Join the output tokens
    for (int i = 0; i < (int)outputTokens.size(); i++) {
        if (i > 0) {
            // If previous token ended with a digit or a diputs character and next starts with a digit, need space?
            // But problem says: "All numbers in the input are non-negative and do not exceed maximum number representable in Diputs notation. It is possible that the numbers will not be separated by space or other characters."
            // So we should preserve original tokenization, i.e., no extra spaces
            // Just join without adding anything
        }
        cout << outputTokens[i];
    }
    cout << endl;
    
    return 0;
}
