//
// Created by flowers on 11/05/24.
//

#include <iostream>
#include <fstream>

void print_file(std::ifstream &datafile) {
    std::string line;
    while (std::getline(datafile, line)) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    //std::ifstream datafile("../data/600K_US_Housing_Properties.csv");
    //std::ifstream datafile("../data/selectData.csv");
    //print_file(datafile);
    //datafile.close();
}