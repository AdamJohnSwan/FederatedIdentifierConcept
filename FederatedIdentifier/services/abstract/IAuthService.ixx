module;
#include <string>

export module IAuthService;
import User;

export class IAuthService
{
public:
	virtual std::string Login(std::string userName) = 0;
	virtual std::string GetToken(std::string userName, std::string state, std::string code) = 0;
	virtual void SetToken(std::string userName, std::string ticket) = 0;
	virtual const User& CheckToken(std::string userName, std::string ticket) = 0;
};