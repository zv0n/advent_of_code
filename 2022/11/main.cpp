#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

class Monkey {
  public:
    Monkey( std::function<uint64_t( uint64_t )> worry_increase_function,
            uint64_t divisor,
            const std::deque<uint64_t> &items,
            const std::pair<uint64_t, uint64_t> &target_monkeys )
        : worry_increase( worry_increase_function ), divisor( divisor ),
          items( items ), target_monkeys( target_monkeys ) {}
    // {target_monkey, worry_level}
    std::pair<uint64_t, uint64_t> performTurn(bool managable) {
        inspected_items++;
        auto worry = items.front();
        items.pop_front();
        worry = worry_increase( worry );
        if(managable) {
            worry /= 3;
        }
        worry = worry % common_divisor;
        if ( worry % divisor == 0 ) {
            return { target_monkeys.first, worry };
        }
        return { target_monkeys.second, worry };
    }

    void addWorry( uint64_t worry ) {
        items.push_back( worry );
    }

    bool hasItems() {
        return !items.empty();
    }
    
    const std::deque<uint64_t> &getItems() {
        return items;
    }

    uint64_t getInspectedItems() {
        return inspected_items;
    }

    void setCommonDivisor(uint64_t divisor) {
        common_divisor = divisor;
    }

  private:
    std::function<uint64_t( uint64_t )> worry_increase;
    uint64_t divisor;
    uint64_t common_divisor;
    std::deque<uint64_t> items;
    const std::pair<uint64_t, uint64_t> target_monkeys;
    uint64_t inspected_items = 0;
};

std::vector<Monkey> getMonkeys( std::ifstream &file ) {
    std::vector<Monkey> ret{};
    std::string tmp;
    std::string str;

    std::function<uint64_t( uint64_t )> worry_increase;
    uint64_t divisor;
    uint64_t common_divisor = 1;
    std::deque<uint64_t> items;
    std::pair<uint64_t, uint64_t> target_monkeys;

    while ( std::getline( file, str ) ) {
        if ( str.empty() ) {
            ret.emplace_back(worry_increase, divisor, items, target_monkeys);
            items.clear();
        }
        std::stringstream ss( str );
        ss >> tmp;
        if(tmp == "Starting") {
            ss >> tmp;
            int tmp_i = 0;
            ss >> tmp_i;
            items.push_back(tmp_i);
            while(ss.peek() == ',') {
                ss >> tmp;
                ss >> tmp_i;
                items.push_back(tmp_i);
            }
        } else if(tmp == "Operation:") {
            ss >> tmp; // new
            ss >> tmp; // =
            ss >> tmp; // old
            char op = 0;
            uint64_t parameter = 0;
            ss >> op;
            ss.get();
            if(ss.peek() == 'o') {
                if(op == '+') {
                    worry_increase = [](uint64_t old) {return old + old;};
                } else {
                    worry_increase = [](uint64_t old) {return old * old;};
                }
            } else {
                ss >> parameter;
                if(op == '+') {
                    worry_increase = [parameter](uint64_t old) {return old + parameter;};
                } else {
                    worry_increase = [parameter](uint64_t old) {return old * parameter;};
                }
            }
        } else if(tmp == "Test:") {
            ss >> tmp; // divisible
            ss >> tmp; // by
            ss >> divisor;
            common_divisor *= divisor;
        } else if(tmp == "If") {
            std::string bool_val;
            ss >> bool_val;
            ss >> tmp; // throw
            ss >> tmp; // to
            ss >> tmp; // monkey
            int monkey_index = 0;
            ss >> monkey_index;
            if(bool_val == "true:") {
                target_monkeys.first = monkey_index;
            } else {
                target_monkeys.second = monkey_index;
            }
        }
    }
    ret.emplace_back(worry_increase, divisor, items, target_monkeys);
    for(auto &monkey : ret) {
        monkey.setCommonDivisor(common_divisor);
    }
    return ret;
}

uint64_t part1(const std::vector<Monkey> &input) {
    auto monkeys = input;
    for(int i = 0; i < 20; i++) {
        for(auto &monkey : monkeys) {
            while(monkey.hasItems()) {
                auto monkey_throw = monkey.performTurn(true);
                monkeys[monkey_throw.first].addWorry(monkey_throw.second);
            }
        }
    }
    std::vector<uint64_t> inspected_items{};
    for(auto &monkey : monkeys) {
        inspected_items.push_back(monkey.getInspectedItems());
    }
    std::sort(inspected_items.begin(), inspected_items.end());
    return inspected_items[inspected_items.size() - 1] * inspected_items[inspected_items.size() - 2];
}

uint64_t part2(const std::vector<Monkey> &input) {
    auto monkeys = input;
    for(int i = 1; i <= 10000; i++) {
        for(auto &monkey : monkeys) {
            while(monkey.hasItems()) {
                auto monkey_throw = monkey.performTurn(false);
                monkeys[monkey_throw.first].addWorry(monkey_throw.second);
            }
        }
    }
    std::vector<uint64_t> inspected_items{};
    size_t i = 0;
    for(auto &monkey : monkeys) {
        inspected_items.push_back(monkey.getInspectedItems());
        i++;
    }
    std::sort(inspected_items.begin(), inspected_items.end());
    return inspected_items[inspected_items.size() - 1] * inspected_items[inspected_items.size() - 2];
}

int main() {
    std::ifstream input_file( "input" );
    auto monkeys = getMonkeys( input_file );
    std::cout << "Monkey business after 20 rounds when you don't worry is \033[91;1m" << part1(monkeys) << "\033[0m."
              << std::endl;
    std::cout << "Monkey business after 10000 rounds when you worry a lot is \033[91;1m" << part2(monkeys) << "\033[0m."
              << std::endl;
}
