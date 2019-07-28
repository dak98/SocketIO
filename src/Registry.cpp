#include <Registry.hpp>

#include <exception>

namespace SocketIO
{

int Registry::add(const int sockfd)
{
    int id = baseId;
    bool foundId = false;    
    for (; id < std::numeric_limits<int>::max(); id++)
	if (idToSockfd.find(id) == idToSockfd.end())
	{
	    idToSockfd.emplace(std::make_pair(id, sockfd));
	    sockfdToId.emplace(std::make_pair(sockfd, id));
	    foundId = true;
	}

    if (!foundId)
	throw std::runtime_error{"No free ID available at the moment"};
    return id;
}

void Registry::removeBySockfd(const int sockfd)
{
    const int id = getIdBySockfd(sockfd);
    idToSockfd.erase(id);
    sockfdToId.erase(sockfd);
}

void Registry::removeById(const int id)
{
    const int sockfd = getSockfdById(id);
    idToSockfd.erase(id);
    sockfdToId.erase(sockfd);
}
    
} // SocketIO
