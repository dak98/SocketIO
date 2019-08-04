#include <Registry.hpp>

#include <exception>

namespace SocketIO
{

int Registry::add(int const sockfd)
{
    int id = baseId;
    bool foundId = false;
    for (; id < std::numeric_limits<int>::max() && !foundId; id++)
	if (idToSockfd.find(id) == idToSockfd.end())
	{
	    idToSockfd.emplace(id, sockfd);
	    sockfdToId.emplace(sockfd, id);
	    foundId = true;
	}

    if (!foundId)
	throw std::runtime_error{"No free ID available at the moment"};
    return id;
}

void Registry::removeBySockfd(int const sockfd)
{
    int const id = getIdBySockfd(sockfd);
    idToSockfd.erase(id);
    sockfdToId.erase(sockfd);
}

void Registry::removeById(int const id)
{
    int const sockfd = getSockfdById(id);
    idToSockfd.erase(id);
    sockfdToId.erase(sockfd);
}

std::vector<int> Registry::getIds() const
{
    std::vector<int> ids(idToSockfd.size());
    int index = 0;
    for (auto const& [id, sockfd] : idToSockfd)
	ids[index++] = id;
    return ids;
}

std::vector<int> Registry::getSockfds() const
{
    std::vector<int> sockfds(sockfdToId.size());
    int index = 0;
    for (auto const& [sockfd, id] : sockfdToId)
	sockfds[index++] = sockfd;
    return sockfds;
}    
    
} // SocketIO
