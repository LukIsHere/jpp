#pragma once
#include "Smutex.hpp"
#include "functional"

namespace jpp{
    template<typename T>
    class chain{
            public:
                chain<T>* forward = nullptr;
                T value;
                int64_t id;
                chain(int64_t idd,T v) : value(v){
                    id=idd;
                }
                //add
                chain<T>* add(int64_t idd,T v){
                    if(idd==id){
                        return nullptr;
                    }else if(forward==nullptr){
                        forward = new chain<T>(idd,v);
                        return forward;
                    }else if(id>idd&&forward->id<idd){
                        chain<T>* old = forward;
                        forward = new chain<T>(idd,v);
                        forward->forward = old;
                        return forward;
                    }else if(id>idd&&forward->id>=idd){
                        return forward->add(idd,v);
                    }else{
                        throw "nie mam opcji";
                    }
                }
            //get
                chain<T>* get(int64_t idd){
                    if(id==idd){
                        return this;
                    }else if(forward==nullptr){
                        return nullptr;
                    }else{
                        return forward->get(idd);
                    }
                }
                bool remove(int64_t idd){
                    //to-do
                    if(forward==nullptr){
                        return false;
                    }else if(forward->id==idd){
                        chain<T>* tmp = forward->forward;
                        delete forward;
                        forward = tmp;
                        return true;
                    }else{
                        return forward->remove(idd);
                    }
                }
                ~chain(){
                    delete forward;
                }

        };

        template<typename T>
        class root{//  low ptr / ust \ hi ptr (no root maybe value)
            public:
                root* low = nullptr;
                root*  hi = nullptr;
                T* value = nullptr;
                int64_t id = 0;
            root(chain<T>* u){
                value = &(u->value);
                id = u->id;
            };
            root(chain<T>* r,int64_t l){
                if(l==0){
                    id = 0;
                    value = nullptr;
                }else if(l==1){
                    id = r->id;
                    value = &(r->value);
                }else if(l==2){
                    hi = new root<T>(r,1);
                    id = r->forward->id;
                    value = &(r->forward->value);
                }else if(l==3){
                    hi = new root<T>(r,1);
                    low = new root<T>(r->forward->forward,1);
                    id = r->forward->id;
                    value = &(r->forward->value);
                }else{
                    int64_t itake = l/2;
                    int64_t lowget = l-itake;
                    int64_t higet =  lowget-1;
                    chain<T>* it = r;
                    for (int64_t i = 0; i < itake-1; i++)
                    {
                        it = it->forward;
                    }
                    hi = new root<T>(r,higet);
                    low = new root<T>(r->forward,lowget);
                    id = it->id;
                    value = &(it->value);
                }
            };
            T* get(int64_t idd){
                if(idd==id) return value;
                else if(idd>id){
                    if(hi==nullptr) return nullptr;
                    else return hi->get(idd);
                }else if(idd<id){
                    if(low==nullptr) return nullptr;
                    else return low->get(idd);
                }
                return nullptr;
            }
            void add(int64_t idd,chain<T>* c){
                if(idd>id){
                    if(hi==nullptr){
                        hi = new root(c);
                    }else{
                        hi->add(idd,c);
                    }
                }
                if(idd<id){
                    if(low==nullptr){
                        low= new root(c);
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
        
        template<typename T>
        class Smap{
            private:
                Lock mutex;
                chain<T>* baseCh = nullptr;
                root<T>* baseRoot = nullptr;
                int count = 0;
                bool oadd;
            public:
                T& operator[](int64_t key){
                    return get(key);
                }
                /// @brief super data class with log(n) speed on when optimized in theory
                /// @param optimizeOnAdd automaticaly optimize map when adding to map (may take some resources)
                Smap(bool optimizeOnAdd=true){
                    oadd = true;
                }
                int size(){
                    return count;
                }
                void optimize(){
                    WriteLock lock(mutex);
                    delete baseRoot;
                    if(baseCh==nullptr){
                        baseRoot=nullptr;
                        return;
                    }
                    baseRoot = new root<T>(baseCh,count);
                }
                void add(int64_t key,T value){
                    WriteLock lock(mutex);
                    if(baseCh==nullptr){
                        baseCh=new chain<T>(key,value);
                        count++;
                        if(baseRoot==nullptr){
                            baseRoot = new root<T>(baseCh);
                        }else{
                            baseRoot->add(key,baseCh);
                        }
                        
                    }else if(baseCh->id>=key){
                        chain<T>* tmp = baseCh->add(key,value);
                        if(tmp!=nullptr){
                            baseRoot->add(key,baseCh->get(key));
                            if(baseRoot==nullptr){
                                baseRoot = new root<T>(tmp);
                            }else{
                                baseRoot->add(key,tmp);
                            }
                            count++;
                        }
                    }else{
                        chain<T>* old = baseCh;
                        baseCh = new chain<T>(key,value);
                        baseCh->forward = old;
                        count++;
                        if(baseRoot==nullptr){
                            baseRoot = new root<T>(baseCh);
                        }else{
                            baseRoot->add(key,baseCh);
                        }
                    };
                    lock.unlock();
                    if(oadd)optimize();
                }
                bool exist(int64_t key){
                    ReadLock lock(mutex);
                    if(baseRoot==nullptr){
                        return (baseRoot!=nullptr);
                    }
                    return(baseRoot->get(key)!=nullptr);
                }
            void remove(int64_t key){
                    WriteLock lock(mutex);
                    if(baseCh->id==key){
                        chain<T>* tmp = baseCh;
                        baseCh = baseCh->forward;
                        delete tmp;
                        count--;
                    }else if(baseCh->remove(key)){
                        count--;
                    };
                    lock.unlock();
                    optimize();
                }
            T& get(int64_t key){
                ReadLock lock(mutex);
                if(!exist(key)){
                    lock.unlock();
                    add(key,T());
                }
                return *(baseRoot->get(key));
                
            }
            void loop(std::function<void(T&)>  f){
                ReadLock lock(mutex);
                chain<T>* p = baseCh;
                while (p!=nullptr)
                {
                    f(p->value);
                }
                
            };
        };
}
