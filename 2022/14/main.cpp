#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::vector<bool>> getMap( std::ifstream &file ) {
    std::vector<std::vector<bool>> ret{};
    std::vector<std::vector<std::pair<int,int>>> rocks{};
    int max_y = 0;
    std::string str;
    char tmp_c;
    while ( std::getline( file, str ) ) {
        rocks.emplace_back();
        int x = 0;
        int y = 0;
        std::stringstream ss( str );
        while(ss >> x) {
            ss >> tmp_c;
            ss >> y;
            if(y > max_y) {
                max_y = y;
            }
            rocks.back().push_back({x,y});
            ss >> tmp_c;
            ss >> tmp_c;
        }
    }
//    std::cout << "Resizing: " << max_y << std::endl;
    ret.resize(max_y+1);
    for(int i = 0; i <= max_y; i++) {
        ret[i].resize(1000, false);
    }
    for(auto &rock_line : rocks) {
        for(size_t i = 0; i < rock_line.size() - 1; i++) {
//            std::cout << "i: " << i << std::endl;
//            std::cout << "X: " << rock_line[i].first << "->" << rock_line[i+1].first << std::endl;
//            std::cout << "Y: " << rock_line[i].second << "->" << rock_line[i+1].second << std::endl;
            int x_diff = rock_line[i+1].first - rock_line[i].first;
            x_diff /= abs(x_diff);
            int y_diff = rock_line[i+1].second - rock_line[i].second;
            y_diff /= abs(y_diff);

            int x = rock_line[i].first;
            int y = rock_line[i].second;
            while(x != rock_line[i+1].first || y != rock_line[i+1].second) {
//                std::cout << "X: " << x << ", Y: " << y << std::endl;
                ret[y][x] = true;
                x += x_diff;
                y += y_diff;
            }
            ret[y][x] = true;
        }
    }
    return ret;
}

bool willRest(std::vector<std::vector<bool>> &map, size_t x_source, size_t y_source) {
    size_t x = x_source;
    size_t y = y_source;
    while(y < map.size() - 1) {
//        std::cout << "CHECKING " << "{" << x << ", " << y << "}" << std::endl;
        if(!map[y+1][x]) {
            y++;
        } else if(!map[y+1][x-1]) {
            y++;
            x--;
        } else if(!map[y+1][x+1]) {
            y++;
            x++;
        } else {
            map[y][x] = true;
            if(x == x_source && y == y_source) {
                return false;
            }
//            std::cout << "TRUE" << std::endl;
            return true;
        }
    }
    return false;
}

size_t part1(const std::vector<std::vector<bool>> &map_in) {
    auto map = map_in;
    size_t result = 0;
    while(willRest(map, 500, 0)) {
        result++;
    }
    return result;
}

size_t part2(const std::vector<std::vector<bool>> &map_in) {
    auto map = map_in;
    map.resize(map.size()+2);
    map[map.size()-2].resize(1000, false);
    map.back().resize(1000, true);
    size_t result = 0;
    while(willRest(map, 500, 0)) {
        result++;
    }
    result++; // the source position sand
    return result;
}

int main() {
    std::ifstream input_file( "input" );
    auto map = getMap( input_file );
    std::cout << "There will be \033[91;1m" << part1(map)
              << "\033[0m grains of sand before it starts falling into the abyss." << std::endl;
    std::cout << "There will be \033[91;1m" << part2(map)
              << "\033[0m grains of sand before it stabilizes." << std::endl;
}
