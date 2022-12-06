#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::string getSignal( std::ifstream &file ) {
    std::string str;
    std::getline( file, str );
    return str;
}

uint64_t findDifferentCharGroup(const std::string &signal, size_t group_size) {
    std::deque<char> different;
    int chars = 0;
    for(size_t i = 0; i < signal.size(); i++) {
        chars++;
        for(size_t j = 0; j < different.size(); j++) {
            if(different[j] == signal[i]) {
                for(size_t k = 0; k <= j; k++) {
                    different.pop_front();
                }
                break;
            }
        }
        different.push_back(signal[i]);
        if(different.size() == group_size) {
            break;
        }
    }
    return chars;
}

uint64_t part1(const std::string &signal) {
    return findDifferentCharGroup(signal, 4);
}

uint64_t part2(const std::string &signal) {
    return findDifferentCharGroup(signal, 14);
}

int main() {
    std::ifstream input_file( "input" );
    auto signal = getSignal( input_file );
    std::cout << "First packet marker appears after \033[91;1m" << findDifferentCharGroup(signal, 4)
              << "\033[0m characters." << std::endl;
    std::cout << "First message marker appears after \033[91;1m" << findDifferentCharGroup(signal, 14)
              << "\033[0m characters." << std::endl;
}
