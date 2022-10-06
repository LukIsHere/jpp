#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "include/jpp.hpp"
#include "include/logger.hpp"
cnsl console("konsola");
bool stop = false;
int main(){
    jpp::json obj("{\"hi\":[4,false]}");
    obj["hi"].arrInclude(0);
    obj["hi"].arrAdd(4);
    obj["hi"].arrAdd(jpp::json("true"));
    obj["hi"][4]="hej";
    std::cout << obj.raw() << std::endl;
}