#include "../include/jpp.hpp"
#include <iostream>
//class json{
        //private:
            //bool deffined = false;
            //bool oov = false;//f obj or t value
            //bool fos = false;//ff object or ft array   or   tf string or ttint
            //void* value = nullptr;
            //int l = 1;
            void jpp::json::decode(std::string raw){
                removeValue();
                if(raw==""){
                    setType(t_undefined);
                    return;
                }
                char start = raw.at(0);
                if(start=='{'){//array
                    setType(t_object);
                    bool reading = false;
                    bool key = true;
                    int curl = 0;
                    int type = 0;
                    int item = 0;
                    char a;
                    std::string keyr;
                    std::string obj;
                    int64_t rawl = raw.length();
                    for(int i = 1;i<rawl-1;i++){
                        a = raw.at(i);
                        if(key){
                            if(!reading){
                                if(a=='"'){
                                    key = true;
                                    reading = true;
                                    //keyr+=a;
                                }
                            }else{
                                if(a=='"'){
                                    key = false;
                                    reading = false;
                                    //keyr+=a;
                                }else{
                                    keyr+=a;
                                }
                            }
                        }else{
                            if(!reading){
                                if(a=='{'){
                                    obj+=a;
                                    reading=true;
                                    curl = 1;
                                    type = t_object;
                                }else if(a=='['){
                                    obj+=a;
                                    reading = true;
                                    curl = 1;
                                    type = t_array;
                                }else if(a=='"'){
                                    obj+=a;
                                    reading = true;
                                    type = t_string;
                                }else if(a=='u'){
                                    obj+=a;
                                    reading = true;
                                    type = t_undefined;
                                }else if(a=='t'||a=='f'){
                                    obj+=a;
                                    reading = true;
                                    type = t_bool;
                                }else if(a!=':'){
                                    obj+=a;
                                    reading = true;
                                    type = t_int;
                                }
                                
                            }else{
                                switch (type)
                                {
                                case t_object:
                                    obj+=a;
                                    if(a=='{')curl++;
                                    if(a=='}')curl--;
                                    if(curl==0){
                                        reading=false;
                                        key=true;
                                        objGet(keyr)->decode(obj);
                                        keyr = "";
                                        obj = "";
                                    }
                                    break;
                                case t_array:
                                    obj+=a;
                                    if(a=='[')curl++;
                                    if(a==']')curl--;
                                    if(curl==0){
                                        reading=false;
                                        key=true;
                                        objGet(keyr)->decode(obj);
                                        keyr = "";
                                        obj = "";
                                    }
                                    break;
                                case t_string:
                                    obj+=a;
                                    if(a=='"'){
                                        reading=false;
                                        key=true;
                                        objGet(keyr)->decode(obj);
                                        keyr = "";
                                        obj = "";
                                    }
                                break;
                                case t_undefined:
                                    if(a==','){
                                        reading=false;
                                        key=true;
                                        objGet(keyr)->decode("undefined");
                                        keyr = "";
                                        obj = "";
                                    }else if(i==rawl-2){
                                        obj+=a;
                                        reading=false;
                                        key=true;
                                        objGet(keyr)->decode("undefined");
                                        keyr = "";
                                        obj = "";
                                    }
                                    break;
                                case t_bool:
                                    if(a==','){
                                        reading=false;
                                        key=true;
                                        if(obj=="t")objGet(keyr)->decode("true");
                                        if(obj=="f")objGet(keyr)->decode("false");
                                        keyr = "";
                                        obj = "";
                                    }else if(i==rawl-2){
                                        obj+=a;
                                        reading=false;
                                        key=true;
                                        if(obj=="t")objGet(keyr)->decode("true");
                                        if(obj=="f")objGet(keyr)->decode("false");
                                        keyr = "";
                                        obj = "";
                                    }
                                    break;
                                case t_int:
                                    if(a==','){
                                        reading=false;
                                        key=true;
                                        objGet(keyr)->decode(obj);
                                        keyr = "";
                                        obj = "";
                                    }else if(i==rawl-2){
                                        obj+=a;
                                        reading=false;
                                        key=true;
                                        objGet(keyr)->decode(obj);
                                        keyr = "";
                                        obj = "";
                                    }else{
                                        obj+=a;
                                    }
                                break;
                                }
                                
                                

                            }
                        }
                    }
                }else if(start=='['){//array
                    setType(t_array);
                    bool reading = false;
                    int curl = 0;
                    int type = 0;
                    int item = 0;
                    char a;
                    std::string obj;
                    int64_t rawl = raw.length();
                    for(int i = 1;i<rawl-1;i++){
                        a=raw.at(i);
                        if(!reading){//destect type (not reading)

                            if(a=='['){//array
                                curl = 1;
                                type = t_array;
                                reading = true;
                                obj+=a;//add
                            }else if(a=='{'){//object
                                curl = 1;
                                type = t_object;
                                reading = true;
                                obj+=a;//add
                            }else  if(a=='"'){//string
                                type = t_string;
                                reading = true;
                                obj+=a;//add
                            }else if(a=='u'){//undeffinied
                                type = t_undefined;
                                reading = true;
                                obj+=a;//add
                            }else if(a=='t'||a=='f'){//undeffinied
                                type = t_bool;
                                reading = true;
                                obj+=a;//add
                            }else if(a==','){
                                
                            }else{//number
                                type = t_int;
                                reading = true;
                                obj+=a;//add
                            }
                            
                        }else{//in read mode
                            switch(type){
                                case t_array:
                                    obj+=a;
                                    if(a=='[')curl++;
                                    if(a==']')curl--;
                                    if(curl==0){
                                        reading = false;
                                        arrGet(item)->decode(obj);
                                        obj = "";
                                        item++;
                                    }
                                break;
                                case t_object:
                                    obj+=a;
                                    if(a=='{')curl++;
                                    if(a=='}')curl--;
                                    if(curl==0){
                                        reading = false;
                                        arrGet(item)->decode(obj);
                                        obj = "";
                                        item++;
                                    }
                                break;
                                case t_string:
                                    obj+=a;
                                    if(a=='"'){
                                        reading = false;
                                        arrGet(item)->decode(obj);
                                        obj = "";
                                        item++;
                                    }
                                break;
                                case t_int:
                                    if(a==','){
                                        reading = false;
                                        arrGet(item)->decode(obj);
                                        obj = "";
                                        item++;
                                    }else{
                                        obj+=a;
                                        if(i==rawl-2){
                                            arrGet(item)->decode(obj);
                                            obj = "";
                                            item++;
                                        }
                                    }
                                break;
                                case t_undefined:
                                    if(a==','){
                                        reading = false;
                                        arrGet(item)->decode("undefined");
                                        obj = "";
                                        item++;
                                    }else{
                                        if(i==rawl-2){
                                            arrGet(item)->decode("undefined");
                                            obj = "";
                                            item++;
                                        }
                                    }
                                break;
                                case t_bool:
                                    if(a==','){
                                        reading = false;
                                        if(obj=="t")arrGet(item)->decode("true");
                                        if(obj=="f")arrGet(item)->decode("false");
                                        obj = "";
                                        item++;
                                    }else{
                                        if(i==rawl-2){
                                            if(obj=="t")arrGet(item)->decode("true");
                                            if(obj=="f")arrGet(item)->decode("false");
                                            obj = "";
                                            item++;
                                        }
                                    }
                                break;
                            }
                        }
                    }
                }else if(start=='"'){//string
                    setType(t_string);
                    std::string out;
                    for (int i = 0; i < raw.length(); i++)
                    {
                        if(raw.at(i)!='"')out+=raw.at(i);
                    }
                    set(out);
                }else if(start>47&&start<58){//number
                    set(int64_t(stoll(raw)));
                }else if(raw=="undefined"){//undefined
                    setType(t_undefined);
                }else if(raw=="true"){//undefined
                    setType(t_bool);

                }else if(raw=="false"){//undefined
                    setType(t_bool);

                }else{
                    std::string out;
                    for (int i = 0; i < raw.length(); i++)
                    {
                        if(raw.at(i)!='"')out+=raw.at(i);
                    }
                }
            }
            void jpp::json::setType(int t){

                if(t!=type()){
                    removeValue();
                    deffined = false;
                    switch (t)
                    {
                    case t_int:
                        value = new int64_t(0);
                        oov = true;
                        fos = true;
                        deffined = true;
                        b = false;
                        break;
                    case t_string:
                        value = new std::string;
                        oov = true;
                        fos = false;
                        deffined = true;
                        b = false;
                        break;
                    case t_array:
                        l = 1;
                        value = new json*[l];
                        ((json**)value)[0] = new json(t_undefined);
                        oov = false;
                        fos = true;
                        deffined = true;
                        b = false;
                        break;
                    case t_object:
                        l = 1;
                        value = new objPair*[l];
                        ((objPair**)value)[0] = new objPair("","");
                        oov = false;
                        fos = false;
                        deffined = true;
                        b = false;
                        break;
                    case  t_bool:
                        value = new bool(false);
                        oov = true;
                        fos = true;
                        deffined = true;
                        b = true;
                    default:
                        deffined = false;
                    }
                }
                
            }
            void jpp::json::arrExpand(int destination){
                setType(t_array);
                if(destination<=l)return;
                int oldL = l;
                l = destination;
                json** newarr = new json*[l];
                for(int i = 0;i<l;i++){
                    if(i<oldL)newarr[i]=((json**)value)[i];
                    else newarr[i] = new json(t_undefined);
                }
                delete (json**)value;
                value = newarr;
            }
            void jpp::json::objExpand(int destination){
                setType(t_object);
                if(destination<=l)return;
                int oldL = l;
                l = destination;
                objPair** newobj = new objPair*[l];
                for(int i = 0;i<l;i++){
                    if(i<oldL)newobj[i]=((objPair**)value)[i];
                    else newobj[i] = new objPair("","");
                }
                delete (objPair**)value;
                value = newobj;
            }
            void jpp::json::removeValue(){
                b = false;
                switch (type())
                    {
                    case t_int:
                        delete (int64_t*)value;
                        break;
                    case t_string:
                        delete (std::string*)value;
                        break;
                    case t_bool:
                        delete (bool*)value;
                        break;
                    case t_array:
                        for(int i = 0;i<l;i++){
                            delete ((json**)value)[i];
                        }
                        delete [] (json**)value;
                        break;
                    case t_object:
                        for(int i = 0;i<l;i++){
                            delete ((objPair**)value)[i];
                        }
                        delete [] (objPair**)value;
                        break;
                    case t_undefined:
                        break;
                    }
                value = nullptr;
            }
        //public:
            jpp::json::json(){
                setType(t_undefined);
            }
            jpp::json::json(json& b){
                decode(b.raw());
            };
            jpp::json::json(std::string raw,bool IKnowWhatImDoing){
                if(!IKnowWhatImDoing){
                    if(raw==""){
                        setType(t_undefined);
                        return;
                    };
                    std::string out;
                    char ver;
                    int ok = 0;
                    int zk = 0;
                    int op = 0;
                    int zp = 0;
                    int p = 0; 
                    bool instr = false;
                    for(int i = 0;i<raw.length();i++){//veryfing json
                        ver = raw.at(i);
                        if(ver=='{')ok++;
                        if(ver=='}')zk++;
                        if(ver=='[')op++;
                        if(ver==']')zp++;
                        if(ver=='"'){
                            p++;
                            instr = !instr;
                        };
                        if(ver!=' '||instr)out+=ver;
                    }
                    if(ok==zk&&op==zp&&p%2==0) decode(out);
                    else setType(t_undefined);

                }else{
                   decode(raw); 
                }
                
            };
            jpp::json::json(int64_t v){
                set(v);
            }
            jpp::json::json(int type){
                setType(type);
            }
            jpp::json& jpp::json::operator[](int i){
                return *arrGet(i);
            };
            jpp::json& jpp::json::operator[](std::string i){
                return *objGet(i);
            };
            void jpp::json::operator=(std::string i){
                set(i);
            };
            void jpp::json::operator=(const char* i){
                set(std::string(i));
            };
            void jpp::json::operator=(int i){
                set(i);
            };
            void jpp::json::operator=(int64_t i){
                set(i);
            };
            void jpp::json::operator=(bool i){
                set(i);
            };
            void jpp::json::operator=(json i){
                set(i);
            };
            //check
            int jpp::json::type(){
                if(b)return t_bool;
                if(!deffined)return t_undefined;
                if(oov&&fos)return t_int;
                if(oov&&!fos)return t_string;
                if(!oov&&fos)return t_array;
                if(!oov&&!fos)return t_object;
                return t_undefined;
            }
            bool jpp::json::typeOf(int t){
                return (type()==t);
            }
            bool jpp::json::objExist(std::string key){
                setType(t_object);
                for(int i = 0;i<l;i++){
                    if(((objPair**)value)[i]->key==key)return true;
                }
                return false;
            }
            bool jpp::json::arrInclude(std::string v){
                setType(t_array);
                for(int i = 0;i<l;i++){
                    if(((json**)value)[i]->type()==t_string){
                        if(((json**)value)[i]->strGet()==v)return true;
                    }
                }
                return false;
            };
            bool jpp::json::arrInclude(int64_t v){
                setType(t_array);
                for(int i = 0;i<l;i++){
                    if(((json**)value)[i]->type()==t_int){
                        if(((json**)value)[i]->intGet()==v)return true;
                    }
                }
                return false;
            };
            //get
                //array
                int jpp::json::arrLength(){
                    return l;
                }
                jpp::json* jpp::json::arrGet(int p){
                    setType(t_array);
                    if(p>=l)arrExpand(p+1);
                    if(((json**)value)[p]==nullptr)((json**)value)[p] = new json("");
                    return ((json**)value)[p];
                }
                void jpp::json::arrLoop(std::function<void(jpp::json*)> f){
                    setType(t_array);
                    for(int i = 0;i<l;i++){
                        f(((json**)value)[i]);
                    }
                }
                //object
                void jpp::json::objLoop(std::function<void(jpp::json*)> f){
                    setType(t_object);
                    for(int i = 0;i<l;i++){
                        f(((objPair**)value)[i]->value);
                    }
                }
                jpp::json* jpp::json::objGet(std::string key){
                    setType(t_object);
                    for(int i = 0;i<l;i++){
                        if(((objPair**)value)[i]->key==key)return ((objPair**)value)[i]->value;
                    }
                    objExpand(l+1);
                    ((objPair**)value)[l-1] = new objPair(key,"");
                    return ((objPair**)value)[l-1]->value;
                }
                //int
                int64_t* jpp::json::intGetP(){
                    setType(t_int);
                    return (int64_t*)value;
                };
                int64_t jpp::json::intGet(){
                    setType(t_int);
                    return *((int64_t*)value);
                };
                bool jpp::json::boolGet(){
                    setType(t_bool);
                    return (bool*)value;
                };
                std::string jpp::json::to_string(){
                    switch (type())
                    {
                    case t_int:
                        return std::to_string(*((int64_t*)value));
                        break;
                    case t_string:
                        if(typeOf(t_string))return *((std::string*)value);
                        break;
                    case t_array:
                        return "[object array]";
                        break;
                    case t_object:
                        return "[object object]";
                        break;
                    }
                    return "undefined";
                }
                //string
                std::string* jpp::json::strGetP(){
                    if(typeOf(t_string))return (std::string*)value;
                    setType(t_string);
                    return (std::string*)value;
                };
                std::string jpp::json::strGet(){
                    setType(t_string);
                    return *((std::string*)value);
                };
            //set
                void jpp::json::set(std::string v){
                    setType(t_string);
                    (*(std::string*)value) = v;
                };
                void jpp::json::set(int64_t v){
                    setType(t_int);
                    (*(int64_t*)value) = v;
                };
                void jpp::json::set(int v){
                    setType(t_int);
                    (*(int64_t*)value) = v;
                };
                void jpp::json::set(bool v){
                    setType(t_bool);
                    (*(bool*)value) = v;
                };
                void jpp::json::set(json v){
                    decode(v.raw());
                };
                void jpp::json::arrAdd(json v){
                    setType(t_array);
                    if(l==1&&arrGet(0)->type()==t_undefined)arrGet(0)->set(v);
                    else arrGet(l)->set(v);
                };
                void jpp::json::arrAdd(std::string v){
                    setType(t_array);
                    if(l==1&&arrGet(0)->type()==t_undefined)arrGet(0)->set(v);
                    else arrGet(l)->set(v);
                };
                void jpp::json::arrAdd(int64_t v){
                    setType(t_array);
                    if(l==1&&arrGet(0)->type()==t_undefined)arrGet(0)->set(v);
                    else arrGet(l)->set(v);
                };
            //export
            std::string jpp::json::raw(){
                if(this==nullptr)return "";
                switch (type())
                    {
                    case t_int:
                        return std::to_string(*((int64_t*)value));
                        break;
                    case t_string:
                        return '"'+(*((std::string*)value))+'"';
                        break;
                    case t_array:{
                        std::string out = "[";
                        bool first = true;
                        for(int i = 0;i<l;i++){
                            if(((json**)value)[i]!=nullptr){
                                if (!first)out+=',';
                                out.append(((json**)value)[i]->raw());
                                first = false;
                            }else{
                            }
                        }
                        out += ']';
                        return out;
                        }break;
                    case t_object:{
                        std::string out = "{";
                        bool first = true;
                        for(int i = 0;i<l;i++){
                            if(((objPair**)value)[i]!=nullptr&&((objPair**)value)[i]->key!=""){
                            if (!first)out+=',';
                            out.append('"'+(((objPair**)value)[i]->key)+'"'+':'+(((objPair**)value)[i]->value->raw()));
                            first = false;
                            }
                        }
                        out += '}';
                        return out;
                        }
                        break;
                        case t_bool:
                            if(boolGet())return "true";
                            else return "false";
                    
                    }
                    return "undefined";
            };
            //destructor
                jpp::json::~json(){
                    removeValue();
                }
    //};
    //class objPair{
        //public:
            //std::string key;
            //jpp::json* value;
            jpp::objPair::objPair(){
                key = "";
                value = new json;
            }
            jpp::objPair::objPair(std::string name,std::string raw){
                key = name;
                value = new json(raw);
            }
            jpp::objPair::~objPair(){
                delete value;
            }
    //};