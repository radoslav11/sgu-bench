#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

const int MAXN = 805;
int man_pref[MAXN][MAXN];
int woman_rank[MAXN][MAXN];
int man_next_pref[MAXN];
int woman_partner[MAXN];
int man_partner[MAXN];
string id_to_man[MAXN];
string id_to_woman[MAXN];

char buf[1 << 16];
int buf_pos = 0, buf_len = 0;

inline int next_char() {
    if (buf_pos == buf_len) {
        buf_pos = 0;
        buf_len = fread(buf, 1, sizeof(buf), stdin);
        if (buf_len == 0) return EOF;
    }
    return (unsigned char)buf[buf_pos++];
}

char str_buf[64];
inline string read_string() {
    int len = 0;
    int c = next_char();
    while (c <= 32) {
        if (c == EOF) return "";
        c = next_char();
    }
    while (c > 32) {
        str_buf[len++] = (char)c;
        c = next_char();
    }
    str_buf[len] = '\0';
    return string(str_buf);
}

inline int read_int() {
    int x = 0;
    int c = next_char();
    while (c <= 32) {
        if (c == EOF) return x;
        c = next_char();
    }
    while (c > 32) {
        x = x * 10 + (c - '0');
        c = next_char();
    }
    return x;
}

int main() {
    int N = read_int();
    if (N == 0) return 0;

    unordered_map<string, int> man_to_id;
    unordered_map<string, int> woman_to_id;
    man_to_id.reserve(N);
    woman_to_id.reserve(N);

    for (int i = 0; i < N; ++i) {
        string m = read_string();
        man_to_id[m] = i;
        id_to_man[i] = m;
        for (int j = 0; j < N; ++j) {
            string w = read_string();
            if (woman_to_id.find(w) == woman_to_id.end()) {
                int wid = woman_to_id.size();
                woman_to_id[w] = wid;
                id_to_woman[wid] = w;
            }
            man_pref[i][j] = woman_to_id[w];
        }
    }

    for (int i = 0; i < N; ++i) {
        string w = read_string();
        int w_id = woman_to_id[w];
        for (int j = 0; j < N; ++j) {
            string m = read_string();
            int m_id = man_to_id[m];
            woman_rank[w_id][m_id] = j;
        }
    }

    for (int i = 0; i < N; ++i) {
        man_next_pref[i] = 0;
        woman_partner[i] = -1;
        man_partner[i] = -1;
    }

    vector<int> free_men;
    free_men.reserve(N);
    for (int i = 0; i < N; ++i) {
        free_men.push_back(i);
    }

    while (!free_men.empty()) {
        int m = free_men.back();
        free_men.pop_back();
        int w = man_pref[m][man_next_pref[m]++];
        int current_partner = woman_partner[w];
        if (current_partner == -1 || woman_rank[w][m] < woman_rank[w][current_partner]) {
            if (current_partner != -1) {
                free_men.push_back(current_partner);
            }
            woman_partner[w] = m;
            man_partner[m] = w;
        } else {
            free_men.push_back(m);
        }
    }

    printf("YES\n");
    for (int i = 0; i < N; ++i) {
        printf("%s %s\n", id_to_man[i].c_str(), id_to_woman[man_partner[i]].c_str());
    }

    return 0;
}
