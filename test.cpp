#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "include/jpp.hpp"
#include "include/logger.hpp"
cnsl console("konsola");
bool stop = false;
int main(){
    jpp::json obj;
    obj.objGet("hi")->arrInclude(0);
    obj.objGet("hi")->arrAdd(4);
    std::cout << obj.raw() << std::endl;
}