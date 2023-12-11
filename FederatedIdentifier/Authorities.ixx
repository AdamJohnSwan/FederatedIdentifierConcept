module;
#include <string>
#include <vector>

export module Authorities;

export struct Authority
{
	std::string Domain;
	std::string ClientId;
	std::string ClientSecret;
	std::string AuthorizeEndpoint;
	std::string TokenEndpoint;
};


export static class Authorities
{
public:
	static const Authority AuthorityList[2];
};


const Authority Authorities::AuthorityList[2] = {
		Authority("localhost", "tbyuFDidLLm270LIwVFiazOqjO3ty1JD", "", "https://localhost:44319/authorize", "https://localhost:44319/token"),
		Authority("auth0.com", "pm4imHRhLti6w9zRR8GpNgroabBckPJb", "", "https://dev-yav8sgimt7xub0dg.us.auth0.com/authorize", "https://dev-yav8sgimt7xub0dg.us.auth0.com/oauth/token")
};