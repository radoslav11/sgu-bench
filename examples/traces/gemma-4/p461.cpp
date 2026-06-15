#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * The task is to convert a wiki-text format into HTML list syntax.
 * Rules:
 * - '#' defines an enumerated list (<ol>).
 * - '*' defines a regular list (<ul>).
 * - Two or more consecutive lines starting with the same marker form a list.
 * - A group of two or more consecutive list elements that start with the same marker
 *   forms a nested list. This rule is applied recursively.
 * - The content of a list element is everything after the first marker character.
 */

struct Item {
    enum Type { TEXT, LIST } type;
    string content;
    char marker;
    vector<Item> sub_items;
};

// Recursively parses the lines (or elements) and groups them into lists or text items.
// The logic for top-level lines and nested list elements is virtually identical.
vector<Item> process(const vector<string>& inputs) {
    vector<Item> results;
    int i = 0;
    int n = inputs.size();
    while (i < n) {
        if (inputs[i].empty()) {
            i++;
            continue;
        }
        
        char marker = inputs[i][0];
        // Check if the current line/element starts with a list marker.
        if (marker == '*' || marker == '#') {
            int j = i;
            // A list is formed by a group of two or more consecutive lines starting with the same marker.
            while (j + 1 < n && !inputs[j + 1].empty() && inputs[j + 1][0] == marker) {
                j++;
            }
            
            if (j > i) {
                // Found a group of two or more lines with the same marker.
                Item list_item;
                list_item.type = Item::LIST;
                list_item.marker = marker;
                
                vector<string> sub_inputs;
                for (int k = i; k <= j; k++) {
                    // The content of the element is the original line with the first marker removed.
                    sub_inputs.push_back(inputs[k].substr(1));
                }
                
                // Recurse to find nested lists within the elements of this list.
                list_item.sub_items = process(sub_inputs);
                results.push_back(list_item);
                i = j + 1;
            } else {
                // Only one line starts with the marker; it does not form a list.
                Item text_item;
                text_item.type = Item::TEXT;
                text_item.content = inputs[i];
                results.push_back(text_item);
                i++;
            }
        } else {
            // The line/element does not start with a marker.
            Item text_item;
            text_item.type = Item::TEXT;
            text_item.content = inputs[i];
            results.push_back(text_item);
            i++;
        }
    }
    return results;
}

// Prints the Item structure in the specified HTML format.
void print(const Item& item) {
    if (item.type == Item::TEXT) {
        cout << item.content << "\n";
    } else {
        cout << (item.marker == '*' ? "<ul>" : "<ol>") << "\n";
        for (const auto& sub : item.sub_items) {
            cout << "<li>" << "\n";
            print(sub);
            cout << "</li>" << "\n";
        }
        cout << (item.marker == '*' ? "</ul>" : "</ol>") << "\n";
    }
}

int main() {
    // Optimization for faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<string> lines;
    string line;
    // Read lines from standard input until EOF.
    while (getline(cin, line)) {
        // Handle possible Windows carriage return (\r).
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }

    if (lines.empty()) return 0;

    // Process the wiki-text.
    vector<Item> results = process(lines);

    // Output the generated HTML.
    for (const auto& item : results) {
        print(item);
    }

    return 0;
}

