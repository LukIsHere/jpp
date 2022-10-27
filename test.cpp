#include <iostream>
#include <string>
#include "jpp.hpp"
int main(){
    jpp::json j = "{\"w\":true}";
    std::cout << j.raw() << std::endl;
}