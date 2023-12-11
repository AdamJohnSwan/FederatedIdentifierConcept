module;
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "deps/httplib.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <string>
#include <boost/json.hpp>

export module HttpHandler;
import AuthorizationReponse;
import TokenResponse;

export AuthorizeReponse GetRequestFromAuthorizeEndpoint()
{
    using namespace httplib;
    Server server;
    AuthorizeReponse response;
    server.Get("/code", [&](const Request& req, Response& res)
    {
        res.set_content("getting token...", "text/plain");
        response.State = req.get_param_value("state");
        response.Code = req.get_param_value("code");
        server.stop();
    });
    server.listen("0.0.0.0", 8081);

    if (response.State == "" || response.Code == "")
    {
        throw std::exception("Invalid response from /authorize endpoint");
    }

    return response;
}

enum UrlState
{
    HOST,
    TARGET
};

export TokenResponse SendRequestToTokenEndpoint(std::string endpoint, std::string clientId, std::string clientSecret, std::string code)
{
    namespace json = boost::json;

    UrlState currentState = HOST;
    std::string host = "";
    std::string target = "/";
    int slashCounter = 0;
    std::string currentLine = "";
    for (int i = 0; i < endpoint.size(); i++)
    {
        char s = endpoint[i];
        switch (currentState)
        {
        case(HOST):
            if (s == '/')
            {
                ++slashCounter;
                if (slashCounter == 3)
                {
                    host = currentLine;
                    currentLine = "";
                    currentState = TARGET;
                }
            }
            currentLine += s;
            break;
        case(TARGET):
            target += s;
            break;
        }
    }

    httplib::Client client(host);
    httplib::Params params;
    params.emplace("client_id", clientId);
    params.emplace("client_secret", clientSecret);
    params.emplace("code", code);
    params.emplace("grant_type", "authorization_code");
    params.emplace("redirect_uri", "https://localhost:8080/code");
    httplib::Result response = client.Post(target, params);
    auto body = response->body;
    if (response->status != 200)
    {
        throw std::exception("Invalid response from authority server");
    }
    auto jv = json::parse(response->body);
    std::string token = json::value_to<std::string>(jv.at("access_token"));
    return TokenResponse(token);
}