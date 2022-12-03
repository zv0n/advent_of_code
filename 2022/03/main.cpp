#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>

using Rucksack = std::pair<std::unordered_set<char>, std::unordered_set<char>>;
std::vector<Rucksack> getRucksacks( std::ifstream &file ) {
    std::vector<Rucksack> ret{};
    std::string str;
    while ( std::getline( file, str ) ) {
        ret.emplace_back();
        for(size_t i = 0; i < str.length()/2; i++) {
            ret.back().first.emplace(str[i]);
        }
        for(size_t i = str.length()/2; i < str.length(); i++) {
            ret.back().second.emplace(str[i]);
        }
    }
    return ret;
}

size_t getItemPriority(char item) {
    if(item <= 'Z') {
        return item - 'A' + 27;
    }
    return item - 'a' + 1;
}

size_t getPriorityPart1(const std::vector<Rucksack> &rucksacks) {
    size_t res = 0;
    for(auto &rucksack : rucksacks) {
        for(auto &item : rucksack.first) {
            if(rucksack.second.find(item) != rucksack.second.end()) {
                res += getItemPriority(item);
                break;
            }
        }
    }
    return res;
}

void findItemsInCommon(const std::unordered_set<char> &a, const std::unordered_set<char> &b, std::unordered_set<char> &result) {
    for(auto &item : a) {
        if(b.find(item) != b.end()) {
            result.emplace(item);
        }
    }
}

size_t getPriorityPart2(const std::vector<Rucksack> &rucksacks) {
    size_t res = 0;
    for(size_t group = 0; group < rucksacks.size(); group += 3) {
        std::unordered_set<char> possibilities{};
        findItemsInCommon(rucksacks[group].first, rucksacks[group+1].first, possibilities);
        findItemsInCommon(rucksacks[group].first, rucksacks[group+1].second, possibilities);
        findItemsInCommon(rucksacks[group].second, rucksacks[group+1].first, possibilities);
        findItemsInCommon(rucksacks[group].second, rucksacks[group+1].second, possibilities);
        std::unordered_set<char> result{};
        findItemsInCommon(rucksacks[group+2].first, possibilities, result);
        findItemsInCommon(rucksacks[group+2].first, possibilities, result);
        findItemsInCommon(rucksacks[group+2].second, possibilities, result);
        findItemsInCommon(rucksacks[group+2].second, possibilities, result);
        if(result.size() != 1) {
            std::cerr << "Something strange is afoot!" << std::endl;
            for(auto &item : result) {
                std::cerr << item << std::endl;
            }
            exit(1);
        }
        res += getItemPriority(*result.begin());
    }
    return res;
}

int main() {
    std::ifstream input_file( "input" );
    auto rucksacks = getRucksacks( input_file );
    int part1 = getPriorityPart1(rucksacks);
    std::cout << "Priority of item types that are in both compartmens is \033[91;1m" << part1
              << "\033[0m." << std::endl;
    int part2 = getPriorityPart2(rucksacks);
    std::cout << "Priority of all Elf groups is \033[91;1m" << part2
              << "\033[0m." << std::endl;
}
