#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

int get_index(char c) {
    if (c == 'a') return 0;
    if (c == 'b') return 1;
    if (c == 'c') return 2;
    if (c == 'd') return 3;
    return -1;
}

int main() {
    int N;
    cin >> N;
    string line;
    getline(cin, line); // consume the newline

    int memory[4] = {0, 0, 0, 0};
    int ptr[4] = {0, 1, 2, 3}; // a->0, b->1, c->2, d->3

    for (int i = 0; i < N; i++) {
        getline(cin, line);
        if (!line.empty() && line.back() == ';') {
            line.pop_back();
        }

        if (line.size() >= 11 && line.substr(0, 7) == "writeln") {
            char var = line[8];
            int idx = get_index(var);
            cout << memory[ptr[idx]] << '\n';
        } else {
            size_t pos = line.find(":=");
            if (pos == string::npos) {
                continue;
            }

            string left = line.substr(0, pos);
            string right = line.substr(pos + 2);

            if (!left.empty() && left.back() == '^') {
                char left_var = left[0];
                int left_idx = get_index(left_var);

                if (!right.empty() && right.back() == '^') {
                    char right_var = right[0];
                    int right_idx = get_index(right_var);
                    memory[ptr[left_idx]] = memory[ptr[right_idx]];
                } else {
                    int value = stoi(right);
                    memory[ptr[left_idx]] = value;
                }
            } else {
                if (!left.empty() && !right.empty()) {
                    char left_var = left[0];
                    int left_idx = get_index(left_var);
                    char right_var = right[0];
                    int right_idx = get_index(right_var);
                    ptr[left_idx] = ptr[right_idx];
                }
            }
        }
    }

    return 0;
}
