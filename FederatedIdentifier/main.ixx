module;
#include <string>
#include <vector>
#include <boost/json.hpp>

export module Main;
import IAuthService;
import User;
import IUserService;
import AuthorizationReponse;

enum State
{
    LOGIN,
    WORK
};

class Main
{
public:
    int Run();
    Main(IAuthService& authService, IUserService& userService);
private:
    IAuthService& authService;
    IUserService& userService;
    std::string DoRequest(const User& user, boost::json::value request);
};

export int main();