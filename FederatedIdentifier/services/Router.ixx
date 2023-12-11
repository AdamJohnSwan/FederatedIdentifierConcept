module;
#include <string>
#include <boost/json.hpp>

export module Router;
import IAuthService;
import IUserService;

export class Router
{
private:
	IAuthService& authService;
	IUserService& userService;
public:
	Router(IAuthService& authService, IUserService& userService);
	std::string RouteRequest(std::string signature, std::string body);
};