#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSV {
private:
    std::vector<std::vector<double>> data;
    int rows;
    int cols;

public:
    CSV() : rows(0), cols(0) {}
    void readCSV(const std::string& filename);
    const std::vector<std::vector<double>>& getData() const;
    int getRows() const;
    int getCols() const;
};