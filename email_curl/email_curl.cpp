#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

int main() {

    std::ifstream file("credentials.txt");
    std::string API_CMD;
    if (file.is_open()) {
        std::getline(file, API_CMD);
        file.close();
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }
    int exitStatus = std::system(API_CMD.c_str());

    //Send email here


    return 0;
}