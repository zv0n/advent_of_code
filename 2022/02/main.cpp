#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

enum RockPaperScissors {
    Rock,
    Paper,
    Scissors
};

enum MyStrategy {
    X,
    Y,
    Z
};

using BattleInput = std::pair<RockPaperScissors, MyStrategy>;
using Battle = std::pair<RockPaperScissors, RockPaperScissors>;
std::vector<BattleInput> getInput( std::ifstream &file ) {
    std::vector<BattleInput> ret{};
    char tmp = 0;
    std::string str;
    RockPaperScissors opopnentStrategy = Rock;
    MyStrategy myStrategy = X;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> tmp;
        switch(tmp) {
            case 'A':
                opopnentStrategy = Rock;
                break;
            case 'B':
                opopnentStrategy = Paper;
                break;
            case 'C':
                opopnentStrategy = Scissors;
                break;
            default:
                std::cerr << "Invalid oponent strategy: '" << tmp << "'" << std::endl;
                exit(1);
                break;
        }
        ss >> tmp;
        ss >> tmp;
        switch(tmp) {
            case 'X':
                myStrategy = X;
                break;
            case 'Y':
                myStrategy = Y;
                break;
            case 'Z':
                myStrategy = Z;
                break;
            default:
                std::cerr << "Invalid player strategy: '" << tmp << "'" << std::endl;
                exit(1);
                break;
        }
        ret.push_back( {opopnentStrategy, myStrategy} );
    }
    return ret;
}

std::vector<Battle> battleStrategy1(const std::vector<BattleInput> &battleInput) {
    std::vector<Battle> battle;
    for(auto &input : battleInput) {
        switch(input.second) {
            case X:
                battle.push_back({input.first, Rock});
                break;
            case Y:
                battle.push_back({input.first, Paper});
                break;
            case Z:
                battle.push_back({input.first, Scissors});
                break;
        }
    }
    return battle;
}

RockPaperScissors getLosingShape(const RockPaperScissors &oponent) {
    switch(oponent) {
        case Rock:
            return Scissors;
        case Paper:
            return Rock;
        case Scissors:
            return Paper;
    }
}

RockPaperScissors getWinningShape(const RockPaperScissors &oponent) {
    switch(oponent) {
        case Rock:
            return Paper;
        case Paper:
            return Scissors;
        case Scissors:
            return Rock;
    }
}

std::vector<Battle> battleStrategy2(const std::vector<BattleInput> &battleInput) {
    std::vector<Battle> battle;
    for(auto &input : battleInput) {
        switch(input.second) {
            case X:
                battle.push_back({input.first, getLosingShape(input.first)});
                break;
            case Y:
                battle.push_back({input.first, input.first});
                break;
            case Z:
                battle.push_back({input.first, getWinningShape(input.first)});
                break;
        }
    }
    return battle;
}

int getBattleScore(const Battle &battle) {
    int score = 0;
    if(battle.second == getWinningShape(battle.first)) {
        score += 6;
    } else if (battle.first == battle.second) {
        score += 3;
    }
    switch(battle.second) {
        case Rock:
            score += 1;
            break;
        case Paper:
            score += 2;
            break;
        case Scissors:
            score += 3;
            break;
    }
    return score;
}

uint64_t part1Func(const std::vector<BattleInput> &input) {
    uint64_t score = 0;
    auto battles = battleStrategy1(input);
    for(auto &battle : battles) {
        score += getBattleScore(battle);
    }
    return score;
}

uint64_t part2Func(const std::vector<BattleInput> &input) {
    uint64_t score = 0;
    auto battles = battleStrategy2(input);
    for(auto &battle : battles) {
        score += getBattleScore(battle);
    }
    return score;
}

int main() {
    std::ifstream input_file( "input" );
    auto battleInput = getInput( input_file );
    auto part1 = part1Func(battleInput);
    std::cout << "With the first strategy, you get score of \033[91;1m" << part1
              << "\033[0m." << std::endl;
    int part2 = part2Func(battleInput);
    std::cout << "With the winning strategy you get score of \033[91;1m" << part2
              << "\033[0m." << std::endl;
}
