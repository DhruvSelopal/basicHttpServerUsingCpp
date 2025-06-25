#ifndef USER_CPP
#define USER_CPP

#include<vector>
#include<iostream>
#include<string>
#include<nlohmann/json.hpp>

using nlohmann::json;

struct User{
    std::string name;
    int age;
    std::vector<std::string> hobbies;
    bool isAdult;
    User(std::string nam,int ag, std::vector<std::string> vec,bool adult):name(nam),age(ag),hobbies(vec){}
    User():name(""),age(0),hobbies({}),isAdult(false){}
};

void to_json(json &j,const User &u){
    j = json{
        {"name", u.name},
        {"age", u.age},
        {"hobbies", u.hobbies},
        {"isAdult", u.isAdult}
    };
}

#endif