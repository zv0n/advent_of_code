#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::vector<int>> getTrees( std::ifstream &file ) {
    std::vector<std::vector<int>> ret{};
    std::string str;
    while ( std::getline( file, str ) ) {
        ret.emplace_back();
        for(auto &tree : str) {
            ret.back().push_back(tree - '0');
        }
    }
    return ret;
}

std::vector<std::vector<int>> getVisibilities(const std::vector<std::vector<int>> &trees) {
    std::vector<std::vector<int>> visibilities;
    visibilities.resize(trees.size());
    for(auto &line : visibilities) {
        line.resize(trees[0].size());
    }

    for(size_t i = 0; i < trees.size(); i++) {
        int max_left = -1;
        int max_right = -1;
        for(size_t j = 0; j < trees[i].size(); j++) {
            auto right_index = trees[0].size() - 1 - j;
            // left
            if(trees[i][j] > max_left) {
                max_left = trees[i][j];
                visibilities[i][j] += 1;
            }
            // right
            if(trees[i][right_index] > max_right) {
                max_right = trees[i][right_index];
                visibilities[i][right_index] += 1;
            }
        }
    }
    for(size_t i = 0; i < trees[0].size(); i++) {
        int max_top = -1;
        int max_bottom = -1;
        for(size_t j = 0; j < trees.size(); j++) {
            auto bottom_index = trees.size() - 1 - j;
            // top
            if(trees[j][i] > max_top) {
                max_top = trees[j][i];
                visibilities[j][i] += 1;
            }
            // bottom
            if(trees[bottom_index][i] > max_bottom) {
                max_bottom = trees[bottom_index][i];
                visibilities[bottom_index][i] += 1;
            }
        }
    }
    return visibilities;
}

int part1(const std::vector<std::vector<int>> &trees) {
    auto visibilities = getVisibilities(trees);
    int visible_trees = 0;
    for(auto &line : visibilities) {
        for(auto &tree : line) {
            if(tree > 0) {
                visible_trees++;
            }
        }
    }
    return visible_trees;
}

int getTreeScore(const std::vector<std::vector<int>> &trees, int y, int x) {
    const auto max_height = trees[y][x];
    int top = 0;
    int left = 0;
    int down = 0;
    int right = 0;
    // top
    for(int i = y-1; i >= 0; i--) {
        top++;
        if(trees[i][x] >= max_height) {
            break;
        }
    }
    // left
    for(int i = x-1; i >= 0; i--) {
        left++;
        if(trees[y][i] >= max_height) {
            break;
        }
    }
    // down
    for(int i = y+1; i < (int)trees.size(); i++) {
        down++;
        if(trees[i][x] >= max_height) {
            break;
        }
    }
    // right
    for(int i = x+1; i < (int)trees[0].size(); i++) {
        right++;
        if(trees[y][i] >= max_height) {
            break;
        }
    }

    return top * left * down * right;
}

int part2(const std::vector<std::vector<int>> &trees) {
    int max_score = 0;
    for(int i = 0; i < (int)trees.size(); i++) {
        for(int j = 0; j < (int)trees[0].size(); j++) {
            auto score = getTreeScore(trees, i, j);
            if(score > max_score) {
                max_score = score;
            }
        }
    }
    return max_score;
}

int main() {
    std::ifstream input_file( "input" );
    auto trees = getTrees( input_file );
    part1(trees);
    std::cout << "There are \033[91;1m" << part1(trees)
              << "\033[0m trees visible from the outside." << std::endl;
    std::cout << "The best tree in the forest has score \033[91;1m" << part2(trees)
              << "\033[0m." << std::endl;
}
