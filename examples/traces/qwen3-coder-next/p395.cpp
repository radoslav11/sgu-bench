#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
using namespace std;

int main() {
    int N;
    cin >> N;
    cin.ignore();

    vector<string> log_records;
    set<string> original_names;
    map<string, int> last_occurrence;

    for (int i = 0; i < N; i++) {
        string line;
        getline(cin, line);
        log_records.push_back(line);
        if (line[0] == '+' || line[0] == '-') {
            string name = line.substr(2);
            original_names.insert(name);
            last_occurrence[name] = i;
        }
    }

    int count = 0;
    set<string> present;
    set<string> temp_present;
    vector<string> result;
    int global_temp_counter = 0;

    for (int i = 0; i < N; i++) {
        string rec = log_records[i];
        if (rec[0] == '+') {
            string name = rec.substr(2);
            if (present.find(name) != present.end()) {
                result.push_back("- " + name);
                present.erase(name);
                count--;
            }
            result.push_back(rec);
            present.insert(name);
            count++;
        } else if (rec[0] == '-') {
            string name = rec.substr(2);
            if (present.find(name) == present.end()) {
                result.push_back("+ " + name);
                present.insert(name);
                count++;
            }
            result.push_back(rec);
            present.erase(name);
            count--;
        } else if (rec[0] == '=') {
            int k = stoi(rec.substr(2));
            if (count < k) {
                int to_insert = k - count;
                vector<string> safe_list;
                for (const string& name : original_names) {
                    if (present.find(name) == present.end() && last_occurrence[name] < i) {
                        safe_list.push_back(name);
                    }
                }
                for (const string& name : safe_list) {
                    if (to_insert <= 0) break;
                    result.push_back("+ " + name);
                    present.insert(name);
                    count++;
                    to_insert--;
                }
                while (to_insert > 0) {
                    string temp_name = "temp" + to_string(global_temp_counter);
                    global_temp_counter++;
                    result.push_back("+ " + temp_name);
                    present.insert(temp_name);
                    temp_present.insert(temp_name);
                    count++;
                    to_insert--;
                }
            } else if (count > k) {
                int to_exit = count - k;
                vector<string> temp_to_exit(temp_present.begin(), temp_present.end());
                for (string t : temp_to_exit) {
                    if (to_exit <= 0) break;
                    result.push_back("- " + t);
                    temp_present.erase(t);
                    present.erase(t);
                    count--;
                    to_exit--;
                }
                vector<string> safe_list;
                for (const string& name : original_names) {
                    if (present.find(name) != present.end() && last_occurrence[name] < i) {
                        safe_list.push_back(name);
                    }
                }
                for (const string& name : safe_list) {
                    if (to_exit <= 0) break;
                    if (present.find(name) == present.end()) continue;
                    result.push_back("- " + name);
                    present.erase(name);
                    count--;
                    to_exit--;
                }
                while (to_exit > 0 && count > 0) {
                    string name = *present.begin();
                    result.push_back("- " + name);
                    present.erase(present.begin());
                    count--;
                    to_exit--;
                }
            }
            result.push_back(rec);
        }
    }

    cout << result.size() << endl;
    for (const string& r : result) {
        cout << r << endl;
    }

    return 0;
}
