module;
#include <string>

export module AuthorizationReponse;

export enum AuhtorizeReponseState
{
	NONE,
	STATE,
	CODE,
	DONE
};

export struct AuthorizeReponse
{
	std::string State;
	std::string Code;
};