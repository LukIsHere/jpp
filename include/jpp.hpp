#pragma once
#include <string>
#include <functional>
#include <fstream>
#include <iostream>
#include <thread>
#define t_undefined 0
#define t_object 1
#define t_array 2
#define t_string 3
#define t_int 4

namespace jpp{

    
    class json{
        private:
            bool deffined = false;
            bool oov = false;//f obj or t value
            bool fos = false;//ff object or ft array   or   tf string or ttint
            void* value = nullptr;
            int l = 1;
            void setType(int t);
            void arrExpand(int destination);
            void objExpand(int destination);
            void removeValue();
        public:
            json();
            json(json& b);
            json(std::string raw,bool IKnowWhatImDoing=false);
            json(int64_t v);
            json(int type);
            void decode(std::string raw);
            //check
            int type();
            bool typeOf(int t);
            bool objExist(std::string key);
            bool arrInclude(std::string v);
            bool arrInclude(int64_t v);
            //get
                //array
                int arrLength();
                json* arrGet(int p);
                void arrLoop(std::function<void(json*)> f);
                //object
                void objLoop(std::function<void(json*)> f);
                json* objGet(std::string key);
                //int
                int64_t* intGetP();
                int64_t intGet();
                std::string to_string();
                //string
                std::string* strGetP();
                std::string strGet();
            //set
                void set(std::string v);
                void set(int64_t v);
                void set(json v);
                void arrAdd(json v);
                void arrAdd(std::string v);
                void arrAdd(int64_t v);
            //export
            std::string raw();
            //destructor
                ~json();
    };
    class objPair{
        public:
            std::string key;
            json* value;
            objPair();
            objPair(std::string name,std::string raw);
            ~objPair();
    };
    class user{//id and data
        public:
            int64_t id = 0;
            json*  data;
            user(int64_t idd,std::string raw){
                id = idd;
                data = new json(raw);
            };
            std::string raw(){
                return std::to_string(id)+':'+data->raw()+';';
            }
            ~user(){
                delete data;
            }
    };
    class chain{//hi to low
        //base for the user (no chain no user)
        // ptr<-obj->ptr
        public:
            chain* forward = nullptr;
            user* usr =  nullptr;
            int64_t id;
            chain(int64_t idd,std::string raw){
                id=idd;
                usr = new user(idd,raw);
            }
        //add
            bool add(int64_t idd,std::string raw){
                if(idd==id){
                    return false;
                }else if(forward==nullptr){
                    forward = new chain(idd,raw);
                    return true;
                }else if(id>idd&&forward->id<idd){
                    chain* old = forward;
                    forward = new chain(idd,raw);
                    forward->forward = old;
                    return true;
                }else if(id>idd&&forward->id>=idd){
                    return forward->add(idd,raw);
                }else{
                    throw "nie mam opcji";
                }
            }
        //get
            chain* get(int64_t idd){
                if(id==idd){
                    return this;
                }else if(forward==nullptr){
                    return nullptr;
                }else{
                    return forward->get(idd);
                }
            }
        //export
            std::string raw(){
                return usr->raw();
            }
            std::string rawCh(){
                if(forward!=nullptr){
                    return raw()+forward->rawCh();
                }else{
                    return raw();
                }
            }
            ~chain(){
                delete usr;
                delete forward;
            }

    };
    class root{//  low ptr / ust \ hi ptr (no root maybe user)
        public:
            root* low = nullptr;
            root*  hi = nullptr;
            user* usr = nullptr;
            int64_t id = 0;
        root(user* u){
            usr = u;
            id = u->id;
        };
        root(chain* r,int64_t l){
            if(l==0){

            }else if(l==1){
                id = r->id;
                usr = r->usr;
            }else if(l==2){
                hi = new root(r,1);
                id = r->forward->id;
                usr = r->forward->usr;
            }else if(l==3){
                hi = new root(r,1);
                low = new root(r->forward->forward,1);
                id = r->forward->id;
                usr = r->forward->usr;
            }else{
                int64_t itake = l/2;
                int64_t lowget = l-itake;
                int64_t higet =  lowget-1;
                chain* it = r;
                for (int64_t i = 0; i < itake-1; i++)
                {
                    it = it->forward;
                }
                hi = new root(r,higet);
                low = new root(r->forward,lowget);
                id = it->id;
                usr = it->usr;
            }
        };
        user* get(int64_t idd){
            if(idd==id) return usr;
            else if(idd>id){
                if(hi==nullptr) return nullptr;
                else return hi->get(idd);
            }else if(idd<id){
                if(low==nullptr) return nullptr;
                else return low->get(idd);
            }
            return nullptr;
        }
        void add(int64_t idd,chain* c){
            if(idd>id){
                if(hi==nullptr){
                    hi = new root(c->usr);
                }else{
                    hi->add(idd,c);
                }
            }
            if(idd<id){
                if(low==nullptr){
                    low= new root(c->usr);
                }else{
                    low->add(idd,c);
                }
            }
        }
        ~root(){
            delete low;
            delete hi;
        }
    };
    class place{
        public:
            int64_t id = 0;
            std::string name = "";
            int64_t score = 0;
            place(){};
            place(int64_t idd,int s,std::string n){
                id = idd;
                score = s;
                name = n;
            };
    };
    class rank{
        public:
            place p[25] = {place()};
            rank(){
            };
            void add(place pl){
                if(pl.score>p[24].score){
                    for(int i = 0;i<25;i++){
                        if(p[i].id==pl.id){
                            p[i]=pl;
                            place temp;
                            for(int i = 0;i<24;i++){
                                if(p[23-i].score<p[24-i].score){
                                    temp = p[23-i];
                                    p[23-i] = p[24-i];
                                    p[24-i] = temp;
                                }
                            }
                            return;
                        }
                    }
                    p[24]=pl;
                    place temp;
                    for(int i = 0;i<24;i++){
                        if(p[23-i].score<p[24-i].score){
                            temp = p[23-i];
                            p[23-i] = p[24-i];
                            p[24-i] = temp;
                        }
                    }
                }
            };
            std::string get(){
                std::string out;
                for(int i = 0;i<25;i++){
                    out.append(std::to_string(i+1)+". "+p[i].name+" : "+std::to_string(p[i].score)+"\n");
                }
                return out;
            }

    };
    class jsDB{//load/write data to file
        private:
            std::string path = "default.txt";
            bool optimized = false;
            chain* baseCh = nullptr;
            root* baseRoot = nullptr;
        public:
            int count = 0;
            jsDB(std::string file){
                path = file;
            }
            void load(){//load data from file
                std::ifstream file(path);
                std::string g;
                std::getline(file,g);
                std::string u;
                char na;
                int64_t ul;
                int64_t gl = g.length();
                std::string help;
                int64_t id;
                bool ida = true;
                for(int i = 0;i<gl;i++){
                    na = g.at(i);
                    if(na==';'){
                        add(u);
                        u="";
                    }else{
                        u+=na;
                    }
                }
                
            }
            void save(){//save data to file
                std::ofstream file(path);
                file.clear();
                file << baseCh->rawCh();
                file.close();
            }
            void backup(std::string p){//save backup data to patch
                std::ofstream file(p);
                file.clear();
                file << baseCh->rawCh();
                file.close();
            }
            void optimize(){//optimize the tree for faster searching(might take a moment depending on size)
                optimized = false;
                delete baseRoot;
                baseRoot = new root(baseCh,count);
                optimized = true;
            }
            void add(int64_t user,std::string raw){
                if(baseCh==nullptr){
                    baseCh=new chain(user,raw);
                    count++;
                }else if(baseCh->id>=user){
                    if(baseCh->add(user,raw)){
                        count++;
                    }
                }else{
                    chain* old = baseCh;
                    baseCh = new chain(user,raw);
                    baseCh->forward = old;
                    count++;
                }
                if(optimized){
                    baseRoot->add(user,baseCh->get(user));
                }
            }
            void add(std::string rawu){
                int64_t rawul = rawu.length();
                std::string idd;
                std::string dta;
                bool iddr = true;
                char a;
                for(int i = 0;i<rawul;i++){
                    a = rawu.at(i);
                    if(iddr){
                        if(a!=':'){
                          idd+=a;  
                        }else{
                            iddr = false;
                        }
                        
                    }else{
                        dta+=a;
                    }
                }
                try{
                  add(std::stoll(idd),dta);  
                }catch(...){

                }
                
            }
            bool exist(int64_t user){
                if(optimized){
                    return(baseRoot->get(user)!=nullptr);
                }else{
                    if(baseCh==nullptr) return false;
                    return (nullptr!=baseCh->get(user));
                }
                return false;
            }
            json* get(int64_t user){
                if(optimized){
                    if(!exist(user)){
                        add(user,"{}");
                    }
                    return baseRoot->get(user)->data;
                }else{
                    if(!exist(user)){
                        add(user,"{}");
                    };
                    return baseCh->get(user)->usr->data;
                }
                return nullptr;
            }
            void loop(std::function<void(user*)>  f){
                chain* p = baseCh;
                for(int i = 0;i<count;i++){
                    f(p->usr);
                    if(p->forward!=nullptr){
                        p = p->forward;
                    }
                }
            };
            rank getRanking(std::string category){
                rank out;
                loop([&out,&category](jpp::user* u){
                    if(u->data->objExist(category)){
                        out.add(jpp::place(u->id,u->data->objGet(category)->intGet(),u->data->objGet("name")->strGet()));
                    }
                });
                return out;
            }
    };
    
    
}
