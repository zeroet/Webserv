#include "epoll.hpp"
#include <iostream>
#include <limits>
//
//Canocial Form
//

//Default Construct
Epoll::Epoll() {}

//Construct for excute
Epoll::Epoll(std::vector<ServerBlock> block) : vecBloc_(block)
{
	init_server_socket();
	std::cout << BLUE << "Webserver Run" << FIN << std::endl;
	epoll_server_manager();
	std::cout << "Webserver Close" << std::endl;
}

//Copy Construct
Epoll::Epoll(const Epoll &other) : vecBloc_(other.vecBloc_), c_(other.c_), epollFd_(other.epollFd_)
{
	*this = other;
}

//Destruct
Epoll::~Epoll() {
    this->close_all_serv_socket();
	}

//
//Epoll utility functions
//

//Init Epoll Server Fd list
void    Epoll::init_server_socket()
{
	int numServerFd = vecBloc_.size();
    int count = 0;

	create_epoll_fd();
	for (int i = 0; i < numServerFd; i++)
	{
		// epoll_add(0);
        if (OK != (epoll_add(vecBloc_[i].getSocketFd())))
			std::cout << RED << "PortNumber [" << vecBloc_[i].getListen() <<
            "] Epoll_Ctl_Add failed" << FIN <<std::endl;
        else
        {
            count++;
            std::cout << GREEN << "PortNumber [" << vecBloc_[i].getListen() <<
            "] Epoll_Ctl_Add Success" << FIN << std::endl;
        }
	}
    if (count == 0)
    {
        close_all_serv_socket();
        exit(1);
    }
}

//Create Epoll Fd
void    Epoll::create_epoll_fd()
{
	this->epollFd_ = epoll_create(MAX_EVENT);
	if (epollFd_ < 0)
	{
		close_all_serv_socket();
		// tmp exit error
		std::cout << "Epoll fail" << std::endl;
		exit(1);
	}
}

//Epoll_ctl ADD function
int    Epoll::epoll_add(int fd)
{

	event ev;
	ev.events = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP;
	ev.data.fd = fd;
	if (epoll_ctl(this->epollFd_, EPOLL_CTL_ADD, fd, &ev) < 0)
	{
		close(fd);
		return (ERROR);
	}
	return (OK);
}

//Epoll create client FD && accept()
int   Epoll::create_clnt_socket(int fd)
{
	int                     size = this->vecBloc_.size();
	struct sockaddr_in    clnt_addr;
	int                     clntFd;
	int                     clntLen = sizeof(clnt_addr);

	for (int i = 0; i < size; i++)
	{
		if (fd == this->vecBloc_[i].getSocketFd())
		{
			clntFd = accept(this->vecBloc_[i].getSocketFd(),
			(struct sockaddr*)&clnt_addr, (socklen_t *)&clntLen);
			if (0 > (clntFd = sock.socket_nonBlock_setting(clntFd)))
				std::cout << "accept() error" << std::endl;
			else
				return (clntFd);
		}
	}
	return (ERROR);

}

//
//Epoll Main management Function
//
void    Epoll::epoll_server_manager()
{
    int                 evCount;
    int                 clntFd;
    event               epEvent[MAX_EVENT];

    while (1)
    {
        evCount = epoll_wait(this->epollFd_, epEvent, MAX_EVENT, TIMEOUT);
        // std::cout << "Epoll event count [ " << evCount <<" ]" << std::endl;
        if (evCount < 0)
        {
            std::cout << "Epoll event count error" << std::endl;
            break ;
        }
        for (int i = 0; i < evCount; i++)
        {
            if ((epEvent[i].events & EPOLLERR) || (epEvent[i].events & EPOLLHUP))
            {
                std::cout << "Epoll event error" << std::endl;
                close(epEvent[i].data.fd);
                continue ;
            }
            // else if (epEvent[i].data.fd == 0)
            // {
            //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            //     return ;
            // }
            else if ((find_server_fd(epEvent[i].data.fd)) == OK)
            {
                clntFd = create_clnt_socket(epEvent[i].data.fd);
                if (clntFd != ERROR)
                {
                    epoll_add(clntFd);
                    std::vector<ServerBlock> serverBlock = make_new_serverblock(epEvent[i].data.fd);
                    this->c_.insert(std::make_pair (clntFd, new Connection(clntFd, serverBlock, this)));
                }
                else
                {
                    std::cout << "accept() error !" << std::endl;
                    close(clntFd);
                    continue ;
                }
            }
            else if(epEvent[i].events & EPOLLIN)
            {
                int fd = epEvent[i].data.fd;
                mapConnection::iterator it = this->c_.find(fd);
                it->second->processRequest();
				it->second->printRequestMsg();
                int ret = check_status_connection(it->second->getStatus());
                if (ret == 1)
                    end_connection(fd);
            }
            else if(epEvent[i].events & EPOLLOUT)
            {
                int fd = epEvent[i].data.fd;
                mapConnection::iterator it = this->c_.find(fd);
                it->second->processResponse();
            }
        }
    }
}

void    Epoll::epoll_Ctl_Mode(int fd, int op)
{
    event ev;

    if (op == EPOLLIN)
    {
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET | EPOLLHUP | EPOLLERR;
        epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev);
    }
    else if (op == EPOLLOUT)
    {
        ev.data.fd = fd;
        ev.events = EPOLLOUT | EPOLLET | EPOLLHUP | EPOLLERR;
        epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev);
    }
    else
        std::cout << "EPOLL CTL FUNC ERROR" << std::endl;
}

//
//Utility Functions for Epoll class
//

int     Epoll::find_server_fd(int fd)
{
	int size = vecBloc_.size();

	for (int i = 0; i < size; i++)
	{
		if (fd == vecBloc_[i].getSocketFd())
			return (OK);
	}
	return (ERROR);
}


void    Epoll::close_all_serv_socket()
{
	int count = this->vecBloc_.size();

    if (c_.size() != 0)
    {
        mapConnection::iterator it = c_.begin();
        mapConnection::iterator it1 = c_.end();
        for (; it != it1; it++)
        {
            close(it->first);
            delete it->second;
            c_.erase(it);
        }
        c_.clear();
    }
	for(int i = 0; i < count; i++)
		close(this->vecBloc_[i].getSocketFd());
	close(this->epollFd_);
	std::cout << GREEN << "All Socket Closed" << FIN << std::endl;
}

// Block class or utile  ????
std::vector<ServerBlock>   Epoll::make_new_serverblock(int fd)
{
    std::vector<ServerBlock> tmp;
	int   size = this->vecBloc_.size();
    unsigned int   portnumber = 0;
    

    for (int tmp = 0; tmp < size; tmp++)
    {
        if (fd == vecBloc_[tmp].getSocketFd())
            portnumber = vecBloc_[tmp].getListen();
    }
    
	for (int i = 0; i < size; i++)
	{
		if (portnumber == vecBloc_[i].getListen())
			tmp.push_back(vecBloc_[i]);
	}
    return tmp;
}

// status == close 
void       Epoll::end_connection(int fd)
{
    mapConnection::iterator it = this->c_.find(fd);
    delete it->second;
    close(it->first);
    c_.erase(it);
}


int        Epoll::check_status_connection(std::string status)
{
    if (status.compare("Keep-Alive"))
        return (0);
    else if (status.compare("Close"))
        return (1);
    else
    {   
        std::cout << "connection status error" << std::endl;
        return (2);
    }
}