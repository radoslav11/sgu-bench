#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <cctype>

class Circuit {
public:
    enum class GateType {
        NOT, AND, NAND, OR, NOR, DFF
    };

    struct Gate {
        GateType type;
        std::vector<std::string> inputs;
        std::string output;
    };

    Circuit() {
        // Initialize DFF initial values to 0
    }

    void addInput(const std::string& junction) {
        inputOrder.push_back(junction);
    }

    void addOutput(const std::string& junction) {
        outputOrder.push_back(junction);
    }

    void addGate(const std::string& output, GateType type, const std::vector<std::string>& inputs) {
        Gate gate;
        gate.output = output;
        gate.type = type;
        gate.inputs = inputs;
        gates.push_back(gate);
        gateMap[output] = static_cast<int>(gates.size() - 1);
    }

    void initializeDFFs() {
        // Set DFF outputs to 0 initially
        for (auto& g : gates) {
            if (g.type == GateType::DFF) {
                currentValues[g.output] = 0;
                nextValues[g.output] = 0;
            }
        }
        // For other gates, we'll compute on first tick
    }

    void processTick(const std::map<std::string, int>& inputValues) {
        // First, update DFF values from previous tick
        for (auto& g : gates) {
            if (g.type == GateType::DFF) {
                nextValues[g.output] = currentValues[g.output];
            }
        }

        // Process all gates in order
        for (const auto& g : gates) {
            if (g.type == GateType::DFF) continue; // Already processed above

            std::vector<int> inputValuesVec;
            for (const auto& in : g.inputs) {
                // Check if input is from current input values or from a gate
                if (inputValues.count(in)) {
                    inputValuesVec.push_back(inputValues.at(in));
                } else {
                    inputValuesVec.push_back(currentValues[in]);
                }
            }

            int result = computeGate(g.type, inputValuesVec);
            nextValues[g.output] = result;
        }

        // Commit next values to current
        for (const auto& g : gates) {
            currentValues[g.output] = nextValues[g.output];
        }
    }

    int getOutputValue(const std::string& output) {
        return currentValues[output];
    }

    void setInputValues(const std::string& line) {
        for (size_t i = 0; i < inputOrder.size(); ++i) {
            if (i < line.size() && line[i] == '1') {
                inputValues[inputOrder[i]] = 1;
            } else {
                inputValues[inputOrder[i]] = 0;
            }
        }
    }

    std::string getOutputString(const std::string& line) {
        std::string result;
        for (const auto& out : outputOrder) {
            result += std::to_string(currentValues[out]);
        }
        return result;
    }

    void tickInputValues() {
        currentInputValues = inputValues;
    }

    int computeGate(GateType type, const std::vector<int>& inputs) {
        switch (type) {
            case GateType::NOT:
                return inputs[0] == 0 ? 1 : 0;
            case GateType::AND: {
                for (int val : inputs) {
                    if (val == 0) return 0;
                }
                return 1;
            }
            case GateType::NAND: {
                for (int val : inputs) {
                    if (val == 0) return 1;
                }
                return 0;
            }
            case GateType::OR: {
                for (int val : inputs) {
                    if (val == 1) return 1;
                }
                return 0;
            }
            case GateType::NOR: {
                for (int val : inputs) {
                    if (val == 1) return 0;
                }
                return 1;
            }
            case GateType::DFF:
                return inputs[0];
            default:
                return 0;
        }
    }

private:
    std::vector<std::string> inputOrder;
    std::vector<std::string> outputOrder;
    std::vector<Gate> gates;
    std::map<std::string, int> gateMap;
    std::map<std::string, int> currentValues;
    std::map<std::string, int> nextValues;
    std::map<std::string, int> currentInputValues;
    std::map<std::string, int> inputValues;
};

bool isIdentifier(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isalnum(c) && c != '_') return false;
    }
    return true;
}

void parseGate(const std::string& line, Circuit& circuit) {
    // Format: j1 = op(j2[, j3...])
    // Or: j1 = op(j2) for NOT/DFF

    size_t eqPos = line.find('=');
    if (eqPos == std::string::npos) return;

    std::string lhs = line.substr(0, eqPos);
    // Trim whitespace
    while (!lhs.empty() && std::isspace(lhs.back())) lhs.pop_back();
    while (!lhs.empty() && std::isspace(lhs.front())) lhs.erase(0, 1);

    size_t parenStart = line.find('(', eqPos);
    if (parenStart == std::string::npos) return;

    size_t parenEnd = line.rfind(')');
    if (parenEnd == std::string::npos || parenEnd <= parenStart) return;

    std::string opStr = line.substr(eqPos + 1, parenStart - eqPos - 1);
    while (!opStr.empty() && std::isspace(opStr.back())) opStr.pop_back();
    while (!opStr.empty() && std::isspace(opStr.front())) opStr.erase(0, 1);

    std::string argsStr = line.substr(parenStart + 1, parenEnd - parenStart - 1);

    // Parse operation type
    Circuit::GateType type;
    if (opStr == "NOT") {
        type = Circuit::GateType::NOT;
    } else if (opStr == "AND") {
        type = Circuit::GateType::AND;
    } else if (opStr == "NAND") {
        type = Circuit::GateType::NAND;
    } else if (opStr == "OR") {
        type = Circuit::GateType::OR;
    } else if (opStr == "NOR") {
        type = Circuit::GateType::NOR;
    } else if (opStr == "DFF") {
        type = Circuit::GateType::DFF;
    } else {
        return;
    }

    // Parse arguments
    std::vector<std::string> inputs;
    std::stringstream ss(argsStr);
    std::string arg;
    while (std::getline(ss, arg, ',')) {
        // Trim whitespace
        while (!arg.empty() && std::isspace(arg.front())) arg.erase(0, 1);
        while (!arg.empty() && std::isspace(arg.back())) arg.pop_back();
        if (!arg.empty()) {
            inputs.push_back(arg);
        }
    }

    circuit.addGate(lhs, type, inputs);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Circuit circuit;
    std::string line;

    // Read first block
    while (std::getline(std::cin, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        // Trim leading whitespace
        while (!line.empty() && std::isspace(line.front())) line.erase(0, 1);

        // Check for INPUT
        if (line.substr(0, 6) == "INPUT(" && line.back() == ')') {
            std::string junction = line.substr(6, line.size() - 7);
            circuit.addInput(junction);
            continue;
        }

        // Check for OUTPUT
        if (line.substr(0, 7) == "OUTPUT(" && line.back() == ')') {
            std::string junction = line.substr(7, line.size() - 8);
            circuit.addOutput(junction);
            continue;
        }

        // Check if it's a gate definition (contains '=')
        if (line.find('=') != std::string::npos) {
            parseGate(line, circuit);
            continue;
        }
    }

    // Read "INPUT VALUES" line
    std::getline(std::cin, line);
    // Ensure it's the INPUT VALUES line
    while (std::cin && (line.empty() || line[0] == '#')) {
        std::getline(std::cin, line);
    }

    circuit.initializeDFFs();

    // Process each input line
    std::vector<std::string> outputs;
    while (std::getline(std::cin, line)) {
        // Skip empty lines
        if (line.empty() || line[0] == '#') continue;

        // If line is not binary, it might be incomplete INPUT VALUES section
        // But the problem states INPUT VALUES section ends with binary lines
        // So we process the line as binary input
        if (!line.empty() && std::all_of(line.begin(), line.end(), [](char c) { return c == '0' || c == '1'; })) {
            circuit.tickInputValues();
            // Set input values for this tick
            circuit.setInputValues(line);
            
            // Process one tick
            circuit.processTick(circuit.currentInputValues);

            // Get output values
            std::string outputLine;
            for (const auto& out : circuit.outputOrder) {
                outputLine += std::to_string(circuit.getOutputValue(out));
            }
            outputs.push_back(outputLine);
        }
    }

    // Output all results
    for (const auto& out : outputs) {
        std::cout << out << '\n';
    }

    return 0;
}
