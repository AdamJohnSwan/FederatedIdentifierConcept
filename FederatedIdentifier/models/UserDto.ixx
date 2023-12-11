module;
#include <string>
#include <vector>
#include <boost/json.hpp>

export module UserDto;
import User;

export class UserDto
{
public:
	UserDto(
		std::string userName,
		Role role,
		std::string domain,
		auto workHourRange,
		int timeZoneOffset
	) : UserName(userName), Role(role), Domain(domain), WorkHourRange(workHourRange), TimeZoneOffset(timeZoneOffset)
	{

	}
	const std::string UserName;
	const Role Role;
	const std::string Domain;
	const std::pair<int, int> WorkHourRange;
	const int TimeZoneOffset;

	static std::vector<UserDto> DeSerializeUsers(boost::json::value userValue)
	{
		namespace json = boost::json;
		if (!userValue.is_array())
		{
			throw std::invalid_argument("user array is not properly formatted.");
		}
		std::vector<UserDto> users;
		auto& array = userValue.as_array();
		for (auto& jv : array)
		{
			auto userName = json::value_to<std::string>(jv.at("userName"));			
			auto role = StringToRole(json::value_to<std::string>(jv.at("role")));	
			auto domain = json::value_to<std::string>(jv.at("domain"));
			std::pair<int,int> workHourRange(json::value_to<int>(jv.at("workHourStart")), json::value_to<int>(jv.at("workHourEnd")));
			int timeZoneOffset = json::value_to<int>(jv.at("timZoneOffset"));

			users.push_back(UserDto(userName, role, domain, workHourRange, timeZoneOffset));
		}
		return users;
	}

	static boost::json::value SerializeUsers(const std::vector<UserDto> users)
	{
		auto list = boost::json::array();
		for (const UserDto& u : users)
		{
			auto userObj = boost::json::object();
			userObj.emplace("userName", u.UserName);
			userObj.emplace("role", RoleToString(u.Role));
			userObj.emplace("domain", u.Domain);
			userObj.emplace("workHourStart", u.WorkHourRange.first);
			userObj.emplace("workHourEnd", u.WorkHourRange.second);
			userObj.emplace("timZoneOffset", u.TimeZoneOffset);
			list.push_back(userObj);
		}
		return list;
	}

	static UserDto ConvertUserToDto(const User& user)
	{
		return UserDto(user.UserName, user.Role, user.Domain, user.WorkHourRange, user.TimeZoneOffset);
	}
};