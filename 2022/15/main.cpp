#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Point {
    int64_t x;
    int64_t y;
};

class Sensor {
public:
    Sensor(Point position): _position(position) {}
    void setBeacon(Point position) {
        _beacon = position;
        _closest_beacon = getDistance(position);
    }
    int64_t getDistance(Point position) const {
        return abs(position.x - _position.x) + abs(position.y - _position.y);
    }
    bool closerThanBeacon(Point position) const {
        return getDistance(position) <= _closest_beacon && (position.x != _beacon.x || position.y != _beacon.y);
    }
    bool knownBeacon(Point position) const {
        return position.x == _beacon.x && position.y == _beacon.y;
    }
    std::pair<int64_t, int64_t> getXLimits() const {
        return {_position.x - _closest_beacon, _position.x + _closest_beacon};
    }
    int64_t getManhattan() const {
        return _closest_beacon;
    }
    Point getPosition() const {
        return _position;
    }
private:
    Point _position;
    Point _beacon;
    int64_t _closest_beacon;
};

std::tuple<std::vector<Sensor>, std::pair<int64_t,int64_t>> getInput( std::ifstream &file ) {
    std::vector<Sensor> sensors{};
    std::pair<int64_t, int64_t> x_limits{0,0};
    bool set_limits = false;
    char tmp_c = 0;
    std::string str;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        int x = 0;
        int y = 0;
        ss >> str;
        ss >> str;
        ss >> tmp_c;
        ss >> tmp_c;
        ss >> x;
        ss >> tmp_c;
        ss >> tmp_c;
        ss >> tmp_c;
        ss >> y;
        sensors.emplace_back(Point{x, y});
        ss >> tmp_c;
        ss >> str;
        ss >> str;
        ss >> str;
        ss >> str;
        ss >> tmp_c;
        ss >> tmp_c;
        ss >> x;
        ss >> tmp_c;
        ss >> tmp_c;
        ss >> tmp_c;
        ss >> y;
        sensors.back().setBeacon({x, y});
        auto limits = sensors.back().getXLimits();
        if(!set_limits) {
            x_limits = limits;
            set_limits = true;
        }
        if(limits.first < x_limits.first) {
            x_limits.first = limits.first;
        }
        if(limits.second > x_limits.second) {
            x_limits.second = limits.second;
        }
    }
    return {sensors, x_limits};
}

int64_t part1(const std::vector<Sensor> &sensors, int64_t x_min, int64_t x_max, int64_t y) {
    int64_t impossible = 0;
    for(int64_t x = x_min; x <= x_max; x++) {
        for(auto &sensor : sensors) {
            if(sensor.closerThanBeacon({x, y})) {
                impossible++;
                break;
            }
        }
    }
    return impossible;
}

int64_t part2(const std::vector<Sensor> &sensors, int64_t x_min, int64_t x_max, int64_t y_min, int64_t y_max) {
    for(auto &sensor : sensors) {
        auto dist = sensor.getManhattan() + 1;
        auto pos = sensor.getPosition();
        for(int64_t x_diff = 0; x_diff <= dist; x_diff++) {
            int64_t y_diff = dist - x_diff;
            std::vector<Point> points = {
                {pos.x - x_diff, pos.y - y_diff},
                {pos.x - x_diff, pos.y + y_diff},
                {pos.x + x_diff, pos.y - y_diff},
                {pos.x + x_diff, pos.y + y_diff}
            };
            for(auto &point : points) {
                if(point.x < x_min || point.y < y_min || point.x > x_max || point.y > y_max) {
                    continue;
                }
                bool valid = true;
                for(auto &sensor2 : sensors) {
                    if(sensor2.closerThanBeacon(point) || sensor2.knownBeacon(point)) {
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    return point.x * 4000000 + point.y;
                }
            }
        }
    }
    return -1;
}

int main() {
    std::ifstream input_file( "input" );
    auto input = getInput( input_file );
    std::cout << "There are \033[91;1m" << part1(std::get<0>(input), std::get<1>(input).first, std::get<1>(input).second, 2000000)
              << "\033[0m invalid beacon positions on line 2000000." << std::endl;
    std::cout << "The tuning frequency of the disstress beacon is \033[91;1m" << part2(std::get<0>(input), 0, 4000000, 0, 4000000)
              << "\033[0m." << std::endl;
    return 0;
}
