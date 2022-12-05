#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

class CraneMovement {
public:
    CraneMovement(int count, int source, int dest): count(count), source(source), dest(dest) {}

    int getCount() const {
        return count;
    }
    int getSource() const {
        return source;
    }
    int getDestination() const {
        return dest;
    }
private:
    int count = 0;
    int source = 0;
    int dest = 0;
};

using Crates = std::vector<std::vector<char>>;

std::pair<Crates, std::vector<CraneMovement>> getInput( std::ifstream &file ) {
    Crates crates{};
    std::string str;
    std::getline( file, str );
    crates.resize(str.length() / 4 + 1);
    do {
        if(str.empty()) {
            break;
        }
        for(size_t i = 0; i < str.length(); i+=4) {
            if(str[i] == '[') {
                crates[i/4].push_back(str[i+1]);
            }
        }
    } while ( std::getline( file, str ) );
    for(auto &crate_line : crates) {
        std::reverse(crate_line.begin(), crate_line.end());
    }
    std::string tmp_s = "";
    std::vector<CraneMovement> instructions{};
    while(std::getline(file, str)) {
        int count = 0;
        int source = 0;
        int dest = 0;
        std::stringstream ss( str );
        ss >> tmp_s;
        ss >> count;
        ss >> tmp_s;
        ss >> source;
        source--;
        ss >> tmp_s;
        ss >> dest;
        dest--;
        instructions.emplace_back(count, source, dest);
    }
    return {crates, instructions};
}

std::string part1(const Crates &crates, const std::vector<CraneMovement> &instructions) {
    auto crates_copy = crates;
    for(auto &instruction : instructions) {
        for(int i = 0; i < instruction.getCount(); i++) {
            crates_copy[instruction.getDestination()].push_back(crates_copy[instruction.getSource()].back());
            crates_copy[instruction.getSource()].pop_back();
        }
    }
    std::string res = "";
    for(auto &crate : crates_copy) {
        res += crate.back();
    }
    return res;
}

std::string part2(const Crates &crates, const std::vector<CraneMovement> &instructions) {
    auto crates_copy = crates;
    for(auto &instruction : instructions) {
        auto source_size = crates_copy[instruction.getSource()].size();
        for(int i = instruction.getCount(); i > 0; i--) {
            crates_copy[instruction.getDestination()].push_back(crates_copy[instruction.getSource()][source_size - i]);
        }
        for(int i = 0; i < instruction.getCount(); i++) {
            crates_copy[instruction.getSource()].pop_back();
        }
    }
    std::string res = "";
    for(auto &crate : crates_copy) {
        res += crate.back();
    }
    return res;
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getInput( input_file );
    std::cout << "After rearranging the crates, the top crates will be \033[91;1m" << part1(input.first, input.second)
              << "\033[0m." << std::endl;
    std::cout << "After rearranging the crates with correct procedure, "
              << "the top crates will be \033[91;1m" << part2(input.first, input.second)
              << "\033[0m." << std::endl;
}
