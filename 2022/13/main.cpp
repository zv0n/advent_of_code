#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

enum Result {
    RIGHT,
    WRONG,
    INCONCLUSIVE,
};

class Packet {
public:
    Packet(bool is_number): _is_number(is_number) {}
    void setNumber(int64_t number) {
        _number = number;
    }
    void addValue(const std::shared_ptr<Packet> &value) {
        _values.push_back(value);
    }
    bool isNumber() const {
        return _is_number;
    }
    int64_t getNumber() const {
        if(!_is_number) {
            return -1;
        }
        return _number;
    }
    Result shouldBeFirst(const std::shared_ptr<Packet> &second) const {
        auto max_size = _values.size() < second->_values.size() ? _values.size() : second->_values.size();
        for(size_t i = 0; i < max_size; i++) {
            if(_values[i]->isNumber() && second->_values[i]->isNumber()) {
                if(_values[i]->getNumber() > second->_values[i]->getNumber()) {
                    return WRONG;
                } else if(_values[i]->getNumber() < second->_values[i]->getNumber()) {
                    return RIGHT;
                }
            } else if(!_values[i]->isNumber() && !second->_values[i]->isNumber()) {
                auto res = _values[i]->shouldBeFirst(second->_values[i]);
                if(res != INCONCLUSIVE) {
                    return res;
                }
            } else if(_values[i]->isNumber() && !second->_values[i]->isNumber()) {
                auto tmp_local = std::make_shared<Packet>(false);
                tmp_local->addValue(std::make_shared<Packet>(true));
                tmp_local->_values.back()->setNumber(_values[i]->getNumber());
                auto res = tmp_local->shouldBeFirst(second->_values[i]);
                if(res != INCONCLUSIVE) {
                    return res;
                }
            } else if(!_values[i]->isNumber() && second->_values[i]->isNumber()) {
                auto tmp_second = std::make_shared<Packet>(false);
                tmp_second->addValue(std::make_shared<Packet>(true));
                tmp_second->_values.back()->setNumber(second->_values[i]->getNumber());
                auto res = _values[i]->shouldBeFirst(tmp_second);
                if(res != INCONCLUSIVE) {
                    return res;
                }
            }
        }
        if(second->_values.size() > _values.size()) {
            return RIGHT;
        } else if (second->_values.size() < _values.size()) {
            return WRONG;
        }
        return INCONCLUSIVE;
    }
    void print() {
        if(_is_number) {
            std::cout << _number;
        } else {
            std::cout << "[";
            if(!_values.empty()) {
                for(size_t i = 0; i < _values.size() - 1; i++) {
                    _values[i]->print();
                    std::cout << ",";
                }
                _values.back()->print();
            }
            std::cout << "]";
        }
    }
private:
    std::vector<std::shared_ptr<Packet>> _values;
    bool _is_number = false;
    int64_t _number = 0;
};

std::vector<std::pair<std::shared_ptr<Packet>, std::shared_ptr<Packet>>> getPackets( std::ifstream &file ) {
    std::vector<std::pair<std::shared_ptr<Packet>, std::shared_ptr<Packet>>> ret{};
    char tmp_c = 0;
    int64_t tmp_i = 0;
    std::string str;
    std::shared_ptr<Packet> left = nullptr;
    std::shared_ptr<Packet> right = nullptr;
    while ( std::getline( file, str ) ) {
        if(str.empty()) {
            ret.push_back({left, right});
            left = nullptr;
            right = nullptr;
            continue;
        }
        std::stringstream ss( str );
        ss >> tmp_c;
        auto packet = std::make_shared<Packet>(false);
        std::vector<std::shared_ptr<Packet>> parents = {packet};
        while(!parents.empty()) {
            if(ss.peek() == '[') {
                ss >> tmp_c;
                auto tmp_packet = std::make_shared<Packet>(false);
                parents.back()->addValue(tmp_packet);
                parents.push_back(tmp_packet);
            } else if (ss.peek() == ']') {
                parents.pop_back();
                ss >> tmp_c;
            } else if (ss.peek() == ',') {
                ss >> tmp_c;
            } else {
                ss >> tmp_i;
                auto tmp_packet = std::make_shared<Packet>(true);
                tmp_packet->setNumber(tmp_i);
                parents.back()->addValue(tmp_packet);
            }
        }
        if(left == nullptr) {
            left = packet;
        } else {
            right = packet;
        }
    }
    ret.push_back({left, right});
    return ret;
}

int64_t part1(const std::vector<std::pair<std::shared_ptr<Packet>,std::shared_ptr<Packet>>> &packets) {
    int64_t result = 0;
    for(size_t i = 0; i < packets.size(); i++) {
        if(packets[i].first->shouldBeFirst(packets[i].second) == RIGHT) {
            result += i+1;
        }
    }
    return result;
}

int64_t part2(const std::vector<std::pair<std::shared_ptr<Packet>,std::shared_ptr<Packet>>> &input) {
    std::vector<std::shared_ptr<Packet>> packets;
    for(auto &in : input) {
        packets.push_back(in.first);
        packets.push_back(in.second);
    }
    auto divider_1 = std::make_shared<Packet>(false);
    auto divider_2 = std::make_shared<Packet>(false);
    auto tmp_packet = std::make_shared<Packet>(false);
    auto value_packet = std::make_shared<Packet>(true);
    value_packet->setNumber(2);
    tmp_packet->addValue(value_packet);
    divider_1->addValue(tmp_packet);
    packets.push_back(divider_1);

    tmp_packet = std::make_shared<Packet>(false);
    value_packet = std::make_shared<Packet>(true);
    value_packet->setNumber(6);
    tmp_packet->addValue(value_packet);
    divider_2->addValue(tmp_packet);
    packets.push_back(divider_2);

    std::sort(packets.begin(), packets.end(),
              [](std::shared_ptr<Packet> a, std::shared_ptr<Packet> b){return a->shouldBeFirst(b) == RIGHT;});

    int64_t result = 1;
    for(size_t i = 0; i < packets.size(); i++) {
        if(packets[i] == divider_1 || packets[i] == divider_2) {
            result *= i + 1;
        }
    }

    return result;
}

int main() {
    std::ifstream input_file( "input" );
    auto packets = getPackets( input_file );
    std::cout << "Sum of indices of pairs in correct order is \033[91;1m" << part1(packets)
              << "\033[0m." << std::endl;
    std::cout << "Product of dividers in ordered packets is \033[91;1m" << part2(packets)
              << "\033[0m." << std::endl;
}
