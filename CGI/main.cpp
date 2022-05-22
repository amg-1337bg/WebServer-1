#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sstream>
#include <sys/select.h>
#include <fstream>
#include "config.hpp"
#include "../request parsing and some basic response/request.hpp"
#include "Uriparser.hpp"

#include "Cgi_request.hpp"

std::string	recv_all(int sock){
	char buffer[1024];
	std::string ret;
	int msg_len;
	
	// while (1)
	// {
		memset(buffer, 0, 1024);
		if ((msg_len = recv(sock, buffer, 1023, MSG_PEEK)) < 0)
			std::cout << "Hey" << std::endl;
		else{
			ret += buffer;
		}
	// }
	return ret;
}

std::string	read_file(std::string root, const char* filename){
	std::ifstream fd;
	if (strcmp(filename, "/") == 0)
		fd.open(root + "/index.html");
	else
		fd.open(root + filename);
	std::cout << root + filename << std::endl;
	if (!fd.is_open())
		std::cout << "Failed" << std::endl;
	std::string ret( (std::istreambuf_iterator<char>(fd) ),
                       (std::istreambuf_iterator<char>()    ) );
	return ret;
}

int main(int ac, char **av, char **envp)
{
	if (ac == 2)
	{
		Cgi_request cgi;
		server def;
		request req;
		struct sockaddr_in addr;
		int sock;
		if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
			return -1;
		// fcntl(sock, F_SETFL, O_NONBLOCK);
		int flag = 1;  
		int opt = sizeof(addr);
		// if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		// 	std::cout << "setsockopt error" << std::endl;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(def.port);
		if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)))
			perror("bind");
		if (listen(sock, 10) < 0)
		{
			std::cout << "Listen failed" << std::endl;
			return 0;
		}
		int new_sock, msg_len;
		std::string msg;
		struct kevent changelist, eventlist;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &flag, sizeof(flag));
		std::string respo;
		while (1)
		{
			std::cout << ">>>>>> Waiting for connection <<<<<<<<" << std::endl;
			if (( new_sock = accept(sock, (struct sockaddr *)&addr, (socklen_t*)&opt)) < 0)
			{
				std::cout << "Accept failed " << new_sock << std::endl;
				return 1;
			}
			std::cout << ">>>>>> connection established <<<<<<<<" << std::endl;
			msg = recv_all(new_sock);
			// std::cout << msg << std::endl;
			std::cout << ">>>>>> The request is <<<<<<" << std::endl;
			std::cout << msg << std::endl;
			request req(msg);
			Uriparser pr(req.getUri());
			std::cout << ">>>>>> The response is <<<<<<" << std::endl;
			if (pr.path.find(".php") != std::string::npos)
				respo = cgi.execute(req.getUri());
			else {
				respo = "HTTP/1.1 200 OK\n";
				respo += read_file(getenv("PWD"), pr.path.c_str());
			}
			std::cout << respo << std::endl;
			send(new_sock, respo.c_str(), respo.length(), 0);
			respo.clear();
		}
	}
	std::cerr << "Config file Needed!" << std::endl;
	return (1);
}