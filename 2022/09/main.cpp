#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

enum Directions {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Command {
    Directions direction;
    int steps;
};

struct Position {
    int x;
    int y;
};

Directions charToDirection(char c) {
    switch(c) {
        case 'R':
            return RIGHT;
        case 'L':
            return LEFT;
        case 'U':
            return UP;
        case 'D':
            return DOWN;
    }
    std::cerr << "Invalid direction" << std::endl;
    exit(1);
}

std::vector<Command> getCommands( std::ifstream &file ) {
    std::vector<Command> ret{};
    int tmp_i = 0;
    char tmp_c = 0;
    std::string str;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> tmp_c;
        ss >> tmp_i;
        ret.push_back({charToDirection(tmp_c), tmp_i});
    }
    return ret;
}

void handleMovement(const Position &head, Position &tail) {
    auto x_diff = head.x - tail.x;
    auto y_diff = head.y - tail.y;
    if(abs(x_diff) > 1 || abs(y_diff) > 1) {
        if(x_diff != 0) {
            x_diff /= abs(x_diff);
        }
        if(y_diff != 0) {
            y_diff /= abs(y_diff);
        }
        tail.x += x_diff;
        tail.y += y_diff;
    }
}

int ropeSimulation(const std::vector<Command> &commands, int rope_length) {
    std::vector<Position> positions;
    positions.resize(rope_length);
    for(int i = 0; i < rope_length; i++) {
        positions[i] = {0, 0};
    }
    std::unordered_map<int, std::unordered_map<int, bool>> visits{};
    visits[0][0] = true;
    for(auto &command : commands) {
        Position addition{0, 0};
        switch(command.direction) {
            case RIGHT:
                addition = {1, 0};
                break;
            case LEFT:
                addition = {-1, 0};
                break;
            case UP:
                addition = {0, -1};
                break;
            case DOWN:
                addition = {0, 1};
                break;
        }
        for(int i = 0; i < command.steps; i++) {
            positions[0].x += addition.x;
            positions[0].y += addition.y;
            for(int i = 1; i < rope_length; i++) {
                handleMovement(positions[i-1], positions[i]);
            }
            visits[positions.back().x][positions.back().y] = true;
        }
    }
    int result = 0;
    for(auto &visit : visits) {
        result += visit.second.size();
    }
    return result;
}

int part1(const std::vector<Command> &commands) {
    return ropeSimulation(commands, 2);
}

int part2(const std::vector<Command> &commands) {
    return ropeSimulation(commands, 10);
}

int main() {
    std::ifstream input_file( "input" );
    auto commands = getCommands( input_file );
    std::cout << "The rope's tail visits \033[91;1m" << part1(commands)
              << "\033[0m positions." << std::endl;
    std::cout << "The long rope's tail visits \033[91;1m" << part2(commands)
              << "\033[0m positions." << std::endl;
}
