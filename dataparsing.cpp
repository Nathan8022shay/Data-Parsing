//============================================================================
// Name        : dataparsing.cpp
// Author      : Nathan Prijatel
// Version     : 03/18/2025
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "json.hpp"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;
using json = nlohmann::json;

vector<vector<string>> readFile(const string &filename) {
    ifstream file(filename);
    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, '\t')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    file.close();
    return data;
}

void saveCSV(const vector<vector<string>> &data, const string &outputFile) {
    ofstream file(outputFile);
    for (const auto &row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}

void saveJSON(const vector<vector<string>> &data, const string &outputFile) {
    json jsonData;
    const auto &headers = data[0];

    for (size_t i = 1; i < data.size(); ++i) {
        json record;
        for (size_t j = 0; j < headers.size(); ++j) {
            record[headers[j]] = data[i][j];
        }
        jsonData.push_back(record);
    }

    ofstream file(outputFile);
    file << jsonData.dump(4);
    file.close();
}

void saveXML(const vector<vector<string>> &data, const string &outputFile) {
    XMLDocument doc;
    XMLNode *root = doc.NewElement("root");
    doc.InsertFirstChild(root);
    const auto &headers = data[0];

    for (size_t i = 1; i < data.size(); ++i) {
        XMLElement *record = doc.NewElement("record");
        for (size_t j = 0; j < headers.size(); ++j) {
            XMLElement *element = doc.NewElement(headers[j].c_str());
            element->SetText(data[i][j].c_str());
            record->InsertEndChild(element);
        }
        root->InsertEndChild(record);
    }
    doc.SaveFile(outputFile.c_str());
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <filename> <-c/-j/-x>" << endl;
        return 1;
    }

    string filename = argv[1];
    string format = argv[2];
    vector<vector<string>> data = readFile(filename);

    if (format == "-c") {
        saveCSV(data, filename.substr(0, filename.find_last_of(".")) + ".csv");
    } else if (format == "-j") {
        saveJSON(data, filename.substr(0, filename.find_last_of(".")) + ".json");
    } else if (format == "-x") {
        saveXML(data, filename.substr(0, filename.find_last_of(".")) + ".xml");
    } else {
        cout << "Failed to convert. Use -c for CSV, -j for JSON, -x for XML." << endl;
        return 1;
    }

    cout << "File successfully converted and saved." << endl;
    return 0;
}
