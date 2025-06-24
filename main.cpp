#include <uwebsockets/App.h>
#include<nlohmann/json.hpp>
#include <stdio.h>
#include "iostream"
#include<string>
#include<unordered_map>

using nlohmann::json ;

int main()
{
    std::unordered_map<std::string,std::string>  * db = new std::unordered_map<std::string,std::string>();
    uWS::App * app = new uWS::App();

    app->post("/main",[db](uWS::HttpResponse<false> * res, uWS::HttpRequest * req){
        json * j = new json();
        res->onAborted([res](){
            std::cout << "Aborted";
        });
        res->onData([res,body=std::string{},j,db](std::string_view chunk , bool isLast) mutable{
            body.append(chunk);
            if(isLast){
                *j = json::parse(body);
                std::unordered_map<std::string,std::string> tempMap;
                tempMap = *j;
                db->insert(tempMap.begin(),tempMap.end());
                for(auto &[key,value] : *db){
                std::cout << std::endl <<  key << " " << value << std::endl;
                }
                res->end("Ended");
            }
        });
    });
    app->listen(9001,[](auto * token){
        if(token) std::cout << "Server listening on 9001";
    });
    app->run();

    return 0;
}