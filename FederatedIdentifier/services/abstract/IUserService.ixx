module;
#include <vector>
#include <memory>
#include <string>

export module IUserService;
import UserDto;
import User;

export class IUserService
{
public:
	virtual const std::vector<User>& ListUsers() = 0;
	virtual const User& GetUser(std::string userName) = 0;
	virtual std::vector<UserDto> ListUsersForTransfer() = 0;
	virtual void EditUsers(std::vector<UserDto> users) = 0;
};