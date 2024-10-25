#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

string decodeBaseAsString(const string& value, int base) {
    string result = "0";
    for (char c : value) {
        int digit = (isdigit(c)) ? c - '0' : toupper(c) - 'A' + 10;
        if (digit >= base) {
            cerr << "Invalid digit '" << c << "' for base " << base << endl;
            throw invalid_argument("Invalid digit for base");
        }
        result = to_string(stoull(result) * base + digit);
    }
    return result;
}


double lagrangeInterpolation(const vector<pair<int, string>>& points) {
    int k = points.size();
    double constantTerm = 0;

    for (int i = 0; i < k; ++i) {
        double term = stod(points[i].second);
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= (0.0 - points[j].first) / (points[i].first - points[j].first);
            }
        }
        constantTerm += term;
    }

    return constantTerm;
}

int main() {
    // Open and read the JSON input file
    ifstream inputFile("input2.json");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input.json file!" << endl;
        return 1;
    }

    json input;
    inputFile >> input;
    inputFile.close();
    cout << "JSON Parsed Successfully!" << endl;

    int n = input["keys"]["n"];
    int k = input["keys"]["k"];
    cout << "n = " << n << ", k = " << k << endl;

    vector<pair<int, string>> points;
    for (const auto& item : input.items()) {
        if (item.key() == "keys") continue;

        int x = stoi(item.key());
        int base = stoi(item.value()["base"].get<string>()); 
        string yValue = item.value()["value"];

        string y = decodeBaseAsString(yValue, base);
        cout << "x = " << x << ", y = " << y << " (base " << base << ")" << endl;
        points.push_back({x, y});
    }

    double secret = lagrangeInterpolation(points);
    cout << "Lagrange Interpolation Result: " << secret << endl;

    cout << "Secret (constant term): " << (int)round(secret) << endl;

    return 0;
}