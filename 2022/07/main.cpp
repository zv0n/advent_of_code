#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_map>

enum FileType {
    DIR,
    ENTRY,
};

class File {
public:
    File(FileType type, size_t size, const std::string &name): _type(type), _size(size), _name(std::move(name)) {}
    void setParent(std::shared_ptr<File> parent) {
        _parent = parent;
    }
    void addChild(std::shared_ptr<File> file, std::shared_ptr<File> parent) {
        _children[file->getName()] = file;
        _children[file->getName()]->setParent(parent);
    }
    size_t getTotalSize() const {
        size_t size = 0;
        for(auto &child : _children) {
            size += child.second->getTotalSize();
        }
        size += _size;
        return size;
    }
    FileType getType() const {
        return _type;
    }
    const std::string &getName() const {
        return _name;
    }
    std::shared_ptr<File> getParent() const {
        return _parent;
    }
    std::shared_ptr<File> getChild(const std::string &name) const {
        if(_children.find(name) != _children.end()) {
            return _children.find(name)->second;
        }
        return nullptr;
    }
    const std::unordered_map<std::string, std::shared_ptr<File>> &getChildren() const {
        return _children;
    }
private:
    FileType _type;
    size_t _size;
    std::string _name;
    std::unordered_map<std::string, std::shared_ptr<File>> _children;
    std::shared_ptr<File> _parent = nullptr;
};

std::shared_ptr<File> getFileSystem( std::ifstream &file ) {
    std::shared_ptr<File> root = std::make_shared<File>(DIR, 0, "/");
    std::string str;
    std::getline( file, str ); // cd /
    std::shared_ptr<File> parent = root;
    while ( std::getline( file, str ) ) {
        if(str[0] != '$') {
            std::stringstream ss( str );

            FileType type = DIR;
            size_t size = 0;
            std::string name = "";
            if(str[0] == 'd') {
                ss >> name;
                ss >> name;
            } else {
                type = ENTRY;
                ss >> size;
                ss >> name;
            }
//            std::cout << "Added (" << (type == DIR ? "DIR" : "FILE") << ", " << size << ", " << name << ") TO PARENT " << parent->getName() << std::endl;
            parent->addChild(std::make_shared<File>(type, size, name), parent);
        } else if (str[2] == 'c') {
            std::string target = str.substr(5);
            if(target[0] == '.') {
                parent = parent->getParent();
            } else {
                parent = parent->getChild(target);
            }
//            std::cout << "Switching to parent " << parent->getName() << std::endl;
        }
    }
    return root;
}

size_t part1(std::shared_ptr<File> filesystem) {
    std::vector<std::shared_ptr<File>> directories = {filesystem};
    size_t result = 0;
    for(size_t i = 0; i < directories.size(); i++) {
        auto total_size = directories[i]->getTotalSize();
        if(total_size <= 100000) {
            result += total_size;
        }
        for(const auto &child : directories[i]->getChildren()) {
            if(child.second->getType() == DIR) {
                directories.push_back(child.second);
            }
        }
    }
    return result;
}

size_t part2(std::shared_ptr<File> filesystem) {
    size_t required_free = 30000000 - (70000000 - filesystem->getTotalSize());

    std::vector<std::shared_ptr<File>> directories = {filesystem};
    size_t min_valid = filesystem->getTotalSize();
    for(size_t i = 0; i < directories.size(); i++) {
        auto total_size = directories[i]->getTotalSize();
        if(total_size >= required_free && total_size < min_valid) {
            min_valid = total_size;
        }
        for(const auto &child : directories[i]->getChildren()) {
            if(child.second->getType() == DIR) {
                directories.push_back(child.second);
            }
        }
    }
    return min_valid;
}

int main() {
    std::ifstream input_file( "input" );
    auto filesystem = getFileSystem( input_file );
    std::cout << "Total sum of directories that are smaller than 100000 is \033[91;1m" << part1(filesystem)
              << "\033[0m." << std::endl;
    std::cout << "The smallest directory required to delete has size \033[91;1m" << part2(filesystem)
              << "\033[0m." << std::endl;
}
