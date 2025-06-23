#include <uwebsockets/App.h>
#include<nlohmann/json.hpp>
#include <stdio.h>
#include "iostream"
#include<string>

int main()
{
    nlohmann::json * json = new nlohmann::json(); 
    auto app = uWS::App();
    app.post("/main", [](uWS::HttpResponse<false> *res, uWS::HttpRequest *req)
             {
                res->onAborted([]() {
                    std::cout << "[ABORTED] Client disconnected before body finished.\n";
                });
                res->onData([res,body = std::string{}](std::string_view chunk,bool isLast) mutable{
                    body.append(chunk);
                    if(isLast){
                        nlohmann::json j = nlohmann::json::parse(body);
                        for(auto &[key,value] : j.items()){
                            std::cout << key << " " << value << std::endl;
                        }
                        res->end("ended");
                    }
                });
             });
    app.listen(9001, [](auto *token)
               {
        if(token) std::cout << "Server listeneing on 9001"; });
    app.run();
    
}