module;
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

export module AuthService;
import IAuthService;
import User;
import IUserService;
import Authorities;
import HttpHandler;
import TokenResponse;

export class AuthService : public IAuthService
{
private:
	std::unordered_map<std::string, std::string> tickets = {};
	std::unordered_map<std::string, std::string> states = {};
	IUserService& userService;

	void CheckHours(User& user)
	{
		
	}

	std::string generateState()
	{
		auto randchar = []() -> char
		{
			const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		int length = 40;
		std::string state(length, 0);
		std::generate_n(state.begin(), length, randchar);
		return state;
	}

public:
	AuthService(IUserService& userService) : userService(userService)
	{}

	std::string Login(std::string userName) override
	{
		const User& user = userService.GetUser(userName);
		Authority authority;
		bool authorityFound = false;
		for (auto& a : Authorities::AuthorityList)
		{
			if (a.Domain == user.Domain)
			{
				authority = a;
				authorityFound = true;
				break;
			}
		}
		if (!authorityFound)
		{
			// there is no reason to tell the user that their domain is not in the list of the valid domains. This might seem confusing to the end user but this way the domain list won't leak.
			throw std::invalid_argument("Invalid user: " + userName);
		}
		auto state = generateState();
		states[userName] = state;
		std::string url = authority.AuthorizeEndpoint 
			+ "?response_type=code&redirect_uri=https://localhost:8080/code&scope=email"
			+ "&client_id=" + authority.ClientId
			+ "&audience=test-api"
			+ "&state=" + states[userName];
		return url;
	}

	std::string GetToken(std::string userName, std::string state, std::string code)
	{
		const User& user = userService.GetUser(userName);
		if (!states.contains(userName) || states[userName] == "")
		{
			throw std::invalid_argument("Invalid state provided.");
		}
		if (state != states[userName])
		{
			throw std::invalid_argument("State does not match. Aborting.");
		}
		states.erase(userName);

		Authority authority;
		bool authorityFound = false;
		for (auto& a : Authorities::AuthorityList)
		{
			if (a.Domain == user.Domain)
			{
				authority = a;
				authorityFound = true;
				break;
			}
		}
		if (!authorityFound)
		{
			throw std::invalid_argument("Invalid user: " + userName);
		}

		TokenResponse response = SendRequestToTokenEndpoint(authority.TokenEndpoint, authority.ClientId, authority.ClientSecret, code);
		SetToken(userName, response.Token);
		return response.Token;
	}

	const User& CheckToken(std::string userName, std::string ticket) override
	{
		const User& user = userService.GetUser(userName);
		if (!tickets.contains(userName))
		{
			throw std::invalid_argument(userName + " is unauthorized.");
		}
		if (tickets[userName] != ticket)
		{
			throw std::invalid_argument(userName + " is unauthorized.");
		}
		return user;
	}

	void SetToken(std::string userName, std::string ticket) override
	{
		const User& user = userService.GetUser(userName);
		tickets[user.UserName] = ticket;
	}
};
