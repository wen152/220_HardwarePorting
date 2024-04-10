#include "read_csv.hpp"

void CSV::readCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }
        data.push_back(row);
        ++rows;
        if (cols == 0) {
            cols = row.size();
        }
    }
    file.close();
}

// 获取数据矩阵
const std::vector<std::vector<double>>& CSV::getData() const {
    return data;
}

// 获取数据矩阵的行数
int CSV::getRows() const {
    return rows;
}

// 获取数据矩阵的列数
int CSV::getCols() const {
    return cols;
}