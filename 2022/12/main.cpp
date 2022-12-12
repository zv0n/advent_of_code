#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Point {
    size_t x;
    size_t y;
};

struct Position {
    int elevation;
    bool visited;
};

struct MazeInput {
    std::vector<std::vector<Position>> maze;
    Point start;
};

MazeInput getMaze( std::ifstream &file ) {
    std::vector<std::vector<Position>> maze{};
    Point start{};
    std::string str;
    int y = -1;
    while ( std::getline( file, str ) ) {
        y++;
        maze.emplace_back();
        int x = -1;
        for ( auto &c : str ) {
            x++;
            switch ( c ) {
            case 'S':
                maze.back().push_back( { 1, false } );
                start.y = y;
                start.x = x;
                break;
            case 'E':
                maze.back().push_back( { 27, false } );
                break;
            default:
                maze.back().push_back( { c - 'a' + 1, false } );
            }
        }
    }
    return { maze, start };
}

struct BFSEntry {
    Point point;
    int path_len;
};

int BFS( std::vector<std::vector<Position>> &maze, const Point &start ) {
    std::deque<BFSEntry> entries = { { start, 0 } };
    maze[start.y][start.x].visited = true;
    while ( !entries.empty() ) {
        for ( int y = -1; y < 2; y++ ) {
            for ( int x = -1; x < 2; x++ ) {
                if ( y != 0 && x != 0 ) {
                    continue;
                }
                Point next_point = { entries.front().point.x + x,
                                     entries.front().point.y + y };
                if ( next_point.y < 0 || next_point.y >= maze.size() ||
                     next_point.x < 0 || next_point.x >= maze[0].size() ) {
                    continue;
                }
                if ( !maze[next_point.y][next_point.x].visited &&
                     ( maze[next_point.y][next_point.x].elevation -
                       maze[entries.front().point.y][entries.front().point.x].elevation ) <= 1 ) {
                    if ( maze[next_point.y][next_point.x].elevation == 27 ) {
                        return entries.front().path_len + 1;
                    }
                    maze[next_point.y][next_point.x].visited = true;
                    entries.push_back(
                        { next_point, 1 + entries.front().path_len } );
                }
            }
        }
        entries.pop_front();
    }
    return -1;
}

int part1( const std::vector<std::vector<Position>> &maze,
           const Point &start ) {
    auto maze_copy = maze;
    return BFS( maze_copy, start );
}

int part2( const std::vector<std::vector<Position>> &maze ) {
    int min = -1;
    for ( size_t y = 0; y < maze.size(); y++ ) {
        for ( size_t x = 0; x < maze[0].size(); x++ ) {
            if ( maze[y][x].elevation == 1 ) {
                auto maze_copy = maze;
                auto path = BFS( maze_copy, { x, y } );
                if ( path != -1 && ( path < min || min == -1 ) ) {
                    min = path;
                }
            }
        }
    }
    return min;
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getMaze( input_file );
    std::cout << "Part 1 result is \033[91;1m"
              << part1( input.maze, input.start ) << "\033[0m." << std::endl;
    std::cout << "Part 2 result is \033[91;1m" << part2( input.maze )
              << "\033[0m." << std::endl;
}
