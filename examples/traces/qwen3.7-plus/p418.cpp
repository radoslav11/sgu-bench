#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cctype>

using namespace std;

struct Token {
    enum Type { KEYWORD, IDENT, SYMBOL, END_OF_FILE } type;
    string val;
    int pos;
};

string to_lower(const string& s) {
    string res = s;
    for (char& c : res) c = tolower((unsigned char)c);
    return res;
}

vector<Token> tokenize(const string& s) {
    vector<Token> tokens;
    int i = 0;
    int n = s.length();
    while (i < n) {
        if (isspace((unsigned char)s[i])) {
            i++;
            continue;
        }
        if (s[i] == '\'') {
            if (i + 2 < n && s[i+2] == '\'') {
                tokens.push_back({Token::SYMBOL, s.substr(i, 3), i});
                i += 3;
            } else {
                i++;
            }
        } else if (isalpha((unsigned char)s[i])) {
            int j = i;
            while (j < n && isalpha((unsigned char)s[j])) j++;
            string word = s.substr(i, j - i);
            string lower_word = to_lower(word);
            Token::Type type = Token::IDENT;
            if (lower_word == "program" || lower_word == "procedure" || lower_word == "function" ||
                lower_word == "forward" || lower_word == "begin" || lower_word == "end" ||
                lower_word == "if" || lower_word == "then" || lower_word == "else" ||
                lower_word == "skip" || lower_word == "peek" || lower_word == "error" ||
                lower_word == "var" || lower_word == "string" || lower_word == "integer" ||
                lower_word == "writeln" || lower_word == "halt" || lower_word == "readln" ||
                lower_word == "inc" || lower_word == "length") {
                type = Token::KEYWORD;
            }
            tokens.push_back({type, lower_word, i});
            i = j;
        } else {
            if (i + 1 < n && s.substr(i, 2) == ":=") {
                tokens.push_back({Token::SYMBOL, ":=", i});
                i += 2;
            } else {
                tokens.push_back({Token::SYMBOL, string(1, s[i]), i});
                i++;
            }
        }
    }
    tokens.push_back({Token::END_OF_FILE, "", n});
    return tokens;
}

struct Seq {
    vector<pair<int, string>> items;
    bool operator<(const Seq& other) const {
        return items < other.items;
    }
};

class Parser {
public:
    vector<Token> tokens;
    int pos = 0;

    Parser(const vector<Token>& t) : tokens(t) {}

    Token peek() {
        if (pos < (int)tokens.size()) return tokens[pos];
        return {Token::END_OF_FILE, "", -1};
    }

    void expect(Token::Type type, const string& val = "") {
        if (pos >= (int)tokens.size() || tokens[pos].type != type || (val != "" && tokens[pos].val != val)) {
            exit(1);
        }
        pos++;
    }

    Token consume() {
        return tokens[pos++];
    }

    string parse_routine_name() {
        Token t = consume();
        if (t.type != Token::IDENT) {
            exit(1);
        }
        return t.val;
    }

    set<Seq> parse_statements() {
        set<Seq> result;
        Seq empty_seq;
        result.insert(empty_seq);

        while (pos < (int)tokens.size()) {
            if (tokens[pos].val == "end" || tokens[pos].val == "else") {
                break;
            }

            if (tokens[pos].val == "if") {
                consume(); // if
                struct Branch {
                    char c;
                    set<Seq> body_seqs;
                };
                vector<Branch> branches;
                bool has_else_error = false;

                while (true) {
                    expect(Token::KEYWORD, "peek");
                    expect(Token::SYMBOL, "=");
                    Token str_tok = consume();
                    char c = str_tok.val[1];

                    expect(Token::KEYWORD, "then");
                    expect(Token::KEYWORD, "begin");

                    set<Seq> body_seqs = parse_statements();
                    expect(Token::KEYWORD, "end");

                    branches.push_back({c, body_seqs});

                    if (pos < (int)tokens.size() && tokens[pos].val == "else") {
                        consume(); // else
                        if (pos < (int)tokens.size() && tokens[pos].val == "error") {
                            consume(); // error
                            expect(Token::SYMBOL, ";");
                            has_else_error = true;
                            break;
                        } else if (pos < (int)tokens.size() && tokens[pos].val == "if") {
                            consume(); // if
                            continue;
                        } else {
                            exit(1);
                        }
                    } else {
                        break;
                    }
                }

                set<Seq> if_seqs;
                for (auto& br : branches) {
                    for (auto& seq : br.body_seqs) {
                        Seq new_seq;
                        new_seq.items.push_back({0, string(1, br.c)});
                        for (auto& item : seq.items) {
                            new_seq.items.push_back(item);
                        }
                        if_seqs.insert(new_seq);
                    }
                }

                if (!has_else_error) {
                    if_seqs.insert(empty_seq);
                }

                set<Seq> next_result;
                for (auto& r_seq : result) {
                    for (auto& i_seq : if_seqs) {
                        Seq combined = r_seq;
                        for (auto& item : i_seq.items) {
                            combined.items.push_back(item);
                        }
                        next_result.insert(combined);
                    }
                }
                result = next_result;

            } else {
                string name = parse_routine_name();
                expect(Token::SYMBOL, ";");

                set<Seq> next_result;
                for (auto& r_seq : result) {
                    Seq combined = r_seq;
                    if (name != "skip" && name != "peek" && name != "error") {
                        combined.items.push_back({1, "<" + name + ">"});
                    }
                    next_result.insert(combined);
                }
                result = next_result;
            }
        }

        return result;
    }

    set<Seq> parse_routine_body() {
        expect(Token::KEYWORD, "begin");
        set<Seq> res = parse_statements();
        expect(Token::KEYWORD, "end");
        expect(Token::SYMBOL, ";");
        return res;
    }
};

string format_seq(const Seq& seq) {
    string res = "";
    string current_term = "";
    for (auto& item : seq.items) {
        if (item.first == 0) {
            current_term += item.second;
        } else {
            if (!current_term.empty()) {
                res += "'" + current_term + "'";
                current_term = "";
            }
            res += item.second;
        }
    }
    if (!current_term.empty()) {
        res += "'" + current_term + "'";
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s, line;
    while (getline(cin, line)) {
        s += line + "\n";
    }

    vector<Token> tokens = tokenize(s);
    
    map<string, int> routine_begins;
    for (int i = 0; i < (int)tokens.size(); i++) {
        if (tokens[i].val == "procedure") {
            string name = tokens[i+1].val;
            if (name != "skip" && name != "peek" && name != "error") {
                if (i + 3 < (int)tokens.size() && tokens[i+2].val == ";" && tokens[i+3].val != "forward") {
                    for (int j = i + 3; j < (int)tokens.size(); j++) {
                        if (tokens[j].val == "begin") {
                            routine_begins[name] = j;
                            break;
                        }
                    }
                }
            }
        }
    }

    vector<string> all_lines;
    for (auto& pair : routine_begins) {
        string name = pair.first;
        Parser parser(tokens);
        parser.pos = pair.second;
        set<Seq> raw_seqs = parser.parse_routine_body();
        set<string> prods;
        for (auto& seq : raw_seqs) {
            prods.insert(format_seq(seq));
        }
        string line_out = "<" + name + ">::=";
        bool first = true;
        for (auto& p : prods) {
            if (!first) line_out += "|";
            line_out += p;
            first = false;
        }
        all_lines.push_back(line_out);
    }

    sort(all_lines.begin(), all_lines.end());
    for (auto& line : all_lines) {
        cout << line << "\n";
    }

    return 0;
}
