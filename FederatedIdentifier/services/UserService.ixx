module;
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

export module UserService;
import IUserService;
import User;
import UserDto;

export class UserService : public IUserService
{
private:
	std::vector<User> users = {
		User("adam", ADMIN, "auth0.com", std::pair<int, int>(0, 24), -5)
	};
public:
	const std::vector<User>& ListUsers() override
	{
		return users;
	}

	const User& GetUser(std::string userName) override
	{
		const User* user = NULL;
		for (auto& u : ListUsers())
		{
			if (u.UserName == userName)
			{
				user = &u;
			}
		}
		if (user == NULL)
		{
			throw std::invalid_argument(userName + " is not valid.");
		}
		return *user;
	}

	std::vector<UserDto> ListUsersForTransfer() override
	{
		std::vector<UserDto> users;
		for (auto& u : ListUsers())
		{
			users.push_back(UserDto::ConvertUserToDto(u));
		}
		return users;
	}

	void EditUsers(std::vector<UserDto> newUsers) override
	{
		for (auto& u : newUsers)
		{
			std::erase_if(users, [&](const User& user)
			{
				return user.UserName == u.UserName;
			});
			users.push_back(User(u.UserName, u.Role, u.Domain, u.WorkHourRange, u.TimeZoneOffset));
		}
	}
};