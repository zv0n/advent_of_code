#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

std::vector<uint64_t> getCalories( std::ifstream &file ) {
    std::vector<uint64_t> ret{};
    ret.push_back({});
    uint64_t tmp = 0;
    std::string str;
    uint64_t calories = 0;
    while ( std::getline( file, str ) ) {
        if(str.empty()) {
            ret.push_back(calories);
            calories = 0;
            continue;
        }
        std::stringstream ss( str );
        ss >> tmp;
        calories += tmp;
    }
    ret.push_back(calories);
    std::sort(ret.begin(), ret.end());
    std::reverse(ret.begin(), ret.end());
    return ret;
}

uint64_t getHighestCaloriesElf(const std::vector<uint64_t> &elves) {
    return elves[0];
}

uint64_t get3HighestCaloriesElf(const std::vector<uint64_t> &elves) {
    return elves[0] + elves[1] + elves[2];
}

int main() {
    std::ifstream input_file( "input" );
    auto elves = getCalories( input_file );
    int part1 = getHighestCaloriesElf(elves);
    std::cout << "The elf carrying the most calories is carrying \033[91;1m" << part1
              << "\033[0m calories." << std::endl;
    int part2 = get3HighestCaloriesElf(elves);
    std::cout << "The 3 elves carrying the most calories are carrying \033[91;1m" << part2
              << "\033[0m calories in total." << std::endl;
}
