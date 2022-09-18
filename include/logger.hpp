//better cout or somethink
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class loading{
    public:
        bool stopv = true;
        loading(){

        }
        void start(){
            stopv = false;
            std::thread l([](bool* stop){
                int state = 0;
                std::cout << " ";
                while (!(*stop))
                {
                    if(*stop)break;
                    switch (state)
                    {
                    case 0:
                        std::cout << "\b-";
                        state++;
                        break;
                    case 1:
                        std::cout << "\b\\";
                        state++;
                        break;
                    case 2:
                        std::cout << "\b|";
                        state++;
                        break;
                    case 3:
                        std::cout << "\b/";
                        state = 0;
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                
            },&stopv);
            l.detach();
        }
        void stop(){
            stopv = true;
            std::cout << std::endl;
        }
        void ok(){
            stopv = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << "\bok" << std::endl;
        }
        void error(){
            stopv = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << "\berror" << std::endl;
        }
        ~loading(){
            stopv = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
};

class cnsl{
    public:
        std::string name = "main";
        cnsl(std::string n){
            name = n;
        }
        template<class T>
        void log(T c){
            std::cout << "[" << name << "] " << c << std::endl;
        }
        template<class T>
        void slog(T c){
            std::cout << "[" << name << "] " << c;
        }
};