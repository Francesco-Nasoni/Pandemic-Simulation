#include <iostream>
#include <fstream>

int main(){
    std::ifstream file("config.txt");
    std::string line;
    int i=0;
    while (std::getline(file, line))
    {
        std::string row;
        double val;
        std::size_t pos = line.find_last_of('=');
        if (line.find_first_of('#') != line.find_first_not_of("\t ") ||
            line.find_first_of('#') == std::string::npos && pos != std::string::npos)
        {
            row = line.substr(pos + 2);

            if (row.find_first_not_of("1234567890.") != std::string::npos)
                throw std::runtime_error{"Invalid setting.txt"};

            val = stod(row);
            std::cout<<i<<". "<< val << '\n';
            i++;
        }
    }
};