#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> getInput( std::ifstream &file ) {
    std::vector<int> ret{};
    int tmp = 0;
    std::string str;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> tmp;
        ret.push_back( tmp );
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getInput( input_file );
    int part1 = input[0];
    int part2 = input[0]+1;
    std::cout << "Part 1 result is \033[91;1m" << part1
              << "\033[0m." << std::endl;
    std::cout << "Part 2 result is \033[91;1m" << part2
              << "\033[0m." << std::endl;
}
