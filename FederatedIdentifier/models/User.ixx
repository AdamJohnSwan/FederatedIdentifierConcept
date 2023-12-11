module;
#include <string>
#include <stdexcept>

export module User;

export enum Role
{
	ADMIN,
	OPERATOR
};

export std::string RoleToString(Role role)
{
	switch (role)
	{
		case(ADMIN):
			return "Admin";
		case(OPERATOR):
			return "Operator";
	}
}

export Role StringToRole(std::string s)
{
	if (s == "Admin")
	{
		return ADMIN;
	}
	else if (s == "Operator")
	{
		return OPERATOR;
	}
	throw std::invalid_argument("Role does not exist.");
}

export class User
{
public:
	User& operator =(const User& u)
	{
		User nu = User(u.UserName, u.Role, u.Domain, u.WorkHourRange, u.TimeZoneOffset);
		return nu;
	}
	User(
		std::string userName,
		Role role,
		std::string domain,
		auto workHourRange,
		int timeZoneOffset
		): UserName(userName), Role(role), Domain(domain), WorkHourRange(workHourRange), TimeZoneOffset(timeZoneOffset)
	{
	}
	const std::string UserName;
	const Role Role;
	const std::string Domain;
	const std::pair<int, int> WorkHourRange;
	const int TimeZoneOffset;
};