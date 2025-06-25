#include <uwebsockets/App.h>
#include<nlohmann/json.hpp>
#include <stdio.h>
#include "iostream"
#include<string>
#include<unordered_map>
#include"userController.cpp"

using nlohmann::json ;


int main()
{
    int id = 100;
    std::unordered_map<int,User*> *userDb = new std::unordered_map<int,User*>;
    std::string routeHandler  = "/main";
    UserController ctrl;
    ctrl.get(routeHandler,userDb);
    ctrl.post(routeHandler,userDb,id);
    ctrl.put(routeHandler,userDb);
    ctrl.del(routeHandler,userDb);
    ctrl.listen(9001);
    ctrl.run();
    return 0;
}