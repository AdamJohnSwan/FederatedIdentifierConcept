module;
// Required for boost
#include <SDKDDKVer.h>

#include <iostream>
#include <boost/json.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "deps/di.hpp"
#include "deps/httplib.h"

module Main;
import AuthService;
import UserService;
import UserDto;
import HttpHandler;


Main::Main(IAuthService& authService, IUserService& userService) : authService(authService), userService(userService)
{
}

int Main::Run()
{
    namespace json = boost::json;
    State currentState = LOGIN;
    std::string userName = "";
    while (true)
    {
        try
        {
            if (currentState == LOGIN)
            {
                std::cout << "Type your username to login\n";
                std::cin >> userName;
                auto authorizeEndpoint = authService.Login(userName);
                std::cout << authorizeEndpoint + '\n';
                std::cout << "Waiting for login to finish...\n";
                AuthorizeReponse response = GetRequestFromAuthorizeEndpoint();
                auto token = authService.GetToken(userName, response.State, response.Code);
                std::cout << "Token: " + token + '\n';
                currentState = WORK;
            }
            if (currentState == WORK)
            {
                std::cout << "Input an object...\n";
                std::string work = "";
                std::cin >> work;
                auto jv = json::parse(work).as_object();
                if (jv.if_contains("logout"))
                {
                    userName = "";
                    currentState = LOGIN;
                    continue;
                }
                auto token = boost::json::value_to<std::string>(jv.at("token"));
                auto& user = authService.CheckToken(userName, token);
                auto result = DoRequest(user, jv);
                std::cout << result + '\n';
            }
        }
        catch (const std::exception& ex)
        {
            std::cout << ex.what();
            std::cout << '\n';
            if (currentState = LOGIN)
            {
                userName = "";
            }
        }
    }
    return 0;
}

std::string Main::DoRequest(const User& user, boost::json::value request)
{
    namespace json = boost::json;

    auto method = json::value_to<std::string>(request.at("method"));
    if (method == "list" && user.Role == ADMIN)
    {
        json::value users = UserDto::SerializeUsers(userService.ListUsersForTransfer());
        return json::serialize(users);
    }

    if (method == "edit" && user.Role == ADMIN)
    {
        auto addList = UserDto::DeSerializeUsers(request.at("users"));
        userService.EditUsers(addList);
        return "";
    }

    if (method == "work" && user.Role == OPERATOR)
    {
        return "All work and no play makes Jack a dull bot.";
    }
    
    throw std::invalid_argument("Request does not contain valid method.");
}

int main()
{
    namespace di = boost::di;
    auto injector = di::make_injector(
        di::bind<IAuthService>.to<AuthService>(),
        di::bind<IUserService>.to<UserService>()
    );
    auto app = injector.create<Main>();

    return app.Run();
}

