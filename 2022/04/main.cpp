#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using ElfPair = std::pair<std::pair<int, int>, std::pair<int, int>>;
std::vector<ElfPair> getElfPairs( std::ifstream &file ) {
    std::vector<ElfPair> ret{};
    char tmp_c = 0;
    std::string str;
    while ( std::getline( file, str ) ) {
        ret.emplace_back();
        std::stringstream ss( str );
        ss >> ret.back().first.first;
        ss >> tmp_c;
        ss >> ret.back().first.second;
        ss >> tmp_c;
        ss >> ret.back().second.first;
        ss >> tmp_c;
        ss >> ret.back().second.second;
    }
    return ret;
}

int getFullyContainedPairs(std::vector<ElfPair> &pairs) {
    int overlaps = 0;
    for(auto &pair : pairs) {
        if((pair.first.first >= pair.second.first && pair.first.second <= pair.second.second) ||
           (pair.second.first >= pair.first.first && pair.second.second <= pair.first.second)) {
            overlaps++;
        }
    }
    return overlaps;
}

int getOverlappingPairs(std::vector<ElfPair> &pairs) {
    int overlaps = 0;
    for(auto &pair : pairs) {
        if((pair.first.first >= pair.second.first && pair.first.first <= pair.second.second) ||
           (pair.second.first >= pair.first.first && pair.second.first <= pair.first.second)) {
            overlaps++;
        }
    }
    return overlaps;
}

int main() {
    std::ifstream input_file( "input" );
    auto pairs = getElfPairs( input_file );
    int part1 = getFullyContainedPairs(pairs);
    std::cout << "There are \033[91;1m" << part1
              << "\033[0m pairs that fully contain other elf's work." << std::endl;
    int part2 = getOverlappingPairs(pairs);
    std::cout << "There are \033[91;1m" << part2
              << "\033[0m pairs with overlapping work." << std::endl;
}
