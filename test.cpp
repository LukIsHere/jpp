#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "include/jpp.hpp"
#include "include/logger.hpp"
cnsl console("konsola");
bool stop = false;
int main(){
    jpp::jsDB db("test.txt");
    db.load();
    db.optimize();
    db.get(16)->objGet("hi")->set(10);
    console.log(db.get(16)->raw());
    console.log(db.get(76)->raw());
    db.optimize();
    console.log("zapisane");
    db.backup("tst.txt");
    db.save();
}