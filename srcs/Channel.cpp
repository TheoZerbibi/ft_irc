#include	"ft_irc.hpp"
#define		PREFIX "&#+!"

Channel::Channel(std::string name, std::string key, bool topicIsOpOnly, int maxUser) :
_operator(),
_users(),
_name(name),
_key(key),
_topicIsOpOnly(topicIsOpOnly),
_maxUser(maxUser)
{
}

Channel::~Channel()
{
}

//	bool	ft_check_channel_name(std::string &channel)
//	{
//		
//	}
