#include<uwebsockets/App.h>
#include<iostream>
#include<vector>
#include<string>
#include<nlohmann/json.hpp>
#include"user.cpp"
#include<unordered_map>

using nlohmann::json;




class UserController{
private:
    uWS::App  app;
public:
// get req function
    void get(std::string routeHandler,std::unordered_map<int,User*> * userDb){
        app.get(routeHandler+"/:id",[userDb](uWS::HttpResponse<false> * res,uWS::HttpRequest * req){
            res->onAborted([res](){
                std::cout << "Aborted";
            });
           int id = std::stoi(std::string(req->getParameter(0))); 
           if((*userDb).count(id) == 0){
            res->end("Id does not exist");
            return;
           }
           json j = *((*userDb)[id]);
           res->end(j.dump());
        });
    }

// Post req function
    void post(std::string routeHandler,std::unordered_map<int,User*> * userDb,int &id){
        app.post(routeHandler,[userDb,&id](uWS::HttpResponse<false> * res, uWS::HttpRequest * req){
            res->onAborted([](){
                std::cout << "Aborted" << std::endl;
            });
            res->onData([res,body=std::string{},&id,userDb](std::string_view chunk,bool isLast) mutable{
                body.append(chunk);
                if(isLast){
                    (*userDb)[id] = new User();
                    json j = json::parse(body);
                    if(j.contains("name")) (*userDb)[id]->name = j["name"];
                    if(j.contains("age")) (*userDb)[id]->age =  int(j["age"]);
                    if(j.contains("hobbies")) (*userDb)[id]->hobbies = j["hobbies"];
                    if(j.contains("isAdult")) (*userDb)[id]->isAdult = j["isAdult"];
                     id++;
                    res->writeStatus("200 ok")->end("Added for id : " + std::to_string(id-1));
                }
            });
            
        });
    }

//put req function  
    void put(std::string routeHandler,std::unordered_map<int,User*> * userDb){
        app.put(routeHandler+"/:id",[userDb](uWS::HttpResponse<false> * res,uWS::HttpRequest * req){
            res->onAborted([](){
                std::cout << "Aborted" << std::endl;
            });
            int id  = std::stoi(std::string(req->getParameter(0)));
            res->onData([res,userDb,id,body=std::string{}](std::string_view chunk,bool isLast) mutable{
                body.append(chunk);
                if(isLast){
                    json j = json::parse(body);
                    if(j.contains("name")) (*userDb)[id]->name = j["name"];
                    if(j.contains("age")) (*userDb)[id]->age =  int(j["age"]);
                    if(j.contains("hobbies")) (*userDb)[id]->hobbies = j["hobbies"];
                    if(j.contains("isAdult")) (*userDb)[id]->isAdult = j["isAdult"];
                    res->writeStatus("200 ok")->end("Updated : " + std::to_string(id));
                }
            });
        });
    }

    void del(std::string routeHandler,std::unordered_map<int,User*> * userDb){
        app.del(routeHandler+"/:id",[userDb](uWS::HttpResponse<false> * res,uWS::HttpRequest * req){
            res->onAborted([res](){
                std::cout << "Aborted";
            });
           int id = std::stoi(std::string(req->getParameter(0))); 
           if((*userDb).count(id) == 0) res->end("Id does not exist");
           delete (*userDb)[id];
           (*userDb).erase(id);
           res->writeStatus("200 ok")->end("Deleted");
        });
    }

    void run(){
        app.run();
    }

    void listen(int portNumber){
        app.listen(portNumber,[portNumber](auto * token){
            if(token) std::cout << "Listening on " << portNumber;
        });
    }
};