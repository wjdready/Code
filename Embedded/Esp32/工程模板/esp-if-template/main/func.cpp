#include <string>
#include <iostream>
#include <vector>


void printHello()
{
    std::string s = "来自C++的妙语: ";
    std::vector<int> vInt;
    vInt.push_back(12);
    std::cout << s << vInt[0] << std::endl;
}