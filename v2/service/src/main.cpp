#include <iostream>
#include <vector>
#include <string>

int main() {
    std::cout << "Hello, world!\n";
    std::vector<std::string> vec{"1", "2"};
    for (const auto& str: vec) {
        std::cout << str << std::endl;
    }
    return 0;
}
