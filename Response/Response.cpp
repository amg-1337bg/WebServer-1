#include "Response.hpp"


Response::Response(std::vector<ServerConfig> &servers) : MIME_types(get_MIME_types()), status_defin(), servers(servers) {
	status_defin[400] = " Bad Request\r\n";
	status_defin[200] = " OK\r\n";
	status_defin[201] = " Created\r\n";
	status_defin[202] = " Accepted\r\n";
	status_defin[204] = " No Content\r\n";
	status_defin[401] = " Unauthorized\r\n";
	status_defin[403] = " Forbidden\r\n";
	status_defin[404] = " Not Found\r\n";
	status_defin[405] = " Method Not Allowed\r\n";
	// std::map<std::string, std::string>::iterator it = MIME_types.begin();
	// while (it != MIME_types.end())
	// {
	// 	std::cout << "[\"" << it->first << "\", \"" << it->second << "\"]" << std::endl;
	// 	it++;
	// }
}


std::string	read_file(std::string root, const char* filename){
	std::ifstream fd;
	if (strcmp(filename, "/") == 0)
		fd.open(root + "/index.php");
	else
		fd.open(root + filename);
	std::cout << root + filename << std::endl;
	if (!fd.is_open())
		std::cout << "Failed" << std::endl;
	std::string ret( (std::istreambuf_iterator<char>(fd) ),
                       (std::istreambuf_iterator<char>()    ) );
	return ret;
}

std::string Response::get_response(request	&req) {
	std::string respo;
	Uriparser pr(req.getUri());
	int i = select_server(req);

	if (!pr.extens.empty()){ // request with file
		std::map<std::string, std::string>::iterator it = MIME_types.find(req.getExtension());
		if (it != MIME_types.end()) { // Regular file
			respo = MIME_response(req, servers[i], it);
		} else { // it's CGI
			respo = CGI_response(req, servers[i]);
		}
	} else { // request with Directory
		respo = Dir_response(req, servers[i]);
	}

	// if (pr.path.find(".php") != std::string::npos) { 
	// 	Cgi_request cgi(req, servers[0]);
	// 	respo = cgi.execute();
	// }
	// else {
	// 	respo = "HTTP/1.1 200 OK\r\n\r\n";
	// 	std::string tmp (req.getHeaders().find("Accept")->second);
	// 	respo += read_file(getenv("PWD"), pr.path.c_str());
	// }
	return respo;
}

int	Response::select_server(request &req){
	std::stringstream ss;
	std::string Host(req.getHeaders().find("Host")->second);
	size_t i = Host.find(":");
	std::vector<int> list_servers_p; // list of Servers with the same port by index
	std::vector<int> list_servers_n; // list of Servers with the same name by index

	ss << Host.begin().base() + i + 1;
	int port;
	ss >> port;
	std::string server_name(Host.begin(), Host.begin() + i);

	for (size_t j = 0; j < servers.size(); j++)  // Looking in servers by Port if there is more than one
	{
		if (servers[j].get_port() == port)
			list_servers_p.push_back(j);
	}
	if (list_servers_p.size() > 1) // there is more than one server with the same port
	{
		for (size_t j = 0; j < list_servers_p.size(); j++) {
			if (servers[list_servers_p[j]].get_name() == server_name)
				list_servers_n.push_back(list_servers_p[j]);
		}
		return list_servers_n[0];
	}
	return list_servers_p[0];
}

std::string	Response::MIME_response(request &req, ServerConfig &server, std::map<std::string, std::string>::iterator &it) {
	std::string respo;
	std::vector<std::string> headers;
	std::string filename(server.get_root() + req.getUri()), length;
	std::stringstream ss;
	std::ifstream file(filename);

	std::cout << "MIME Filename = " << filename << std::endl;
	if (file.is_open())
	{
		std::string content((std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ));
		ss << content.length();
		ss >> length;
		headers.push_back("Content-Type: " + it->second);
		headers.push_back("Content-Length: " + length);
		return (make_response(200, headers, content));
	}
	return std::string();
}

std::string	Response::CGI_response(request &req, ServerConfig &server){
	std::string respo;
	Uriparser pr(req.getUri());
	std::string filename(server.get_root() + pr.path);

	std::cout << "CGI Filename = " << filename << std::endl;
	Cgi_request cgi(req, server);
	return cgi.execute();
}

std::string	Response::Dir_response(request &req, ServerConfig &server){
	std::string respo;
	std::string filename(server.get_root() + req.getUri());

	std::cout << "Dir Filename = " << filename << std::endl;

	return std::string();
}

std::string	Response::make_response(int status, std::vector<std::string> &headers, std::string &body) {
	std::string response("HTTP/1.1 ");

	response += status_code(status);
	for(int i = 0; i < headers.size(); i++) {
		response += headers[i] + "\r\n";
	}
	response += "\r\n" + body;
	return response;
}

std::string	Response::status_code(int status){
	std::stringstream ss;
	std::string tmp;
	std::map<int, std::string>::iterator it = status_defin.find(status);
	if (it != status_defin.end())
	{
		ss << it->first;
		ss >> tmp;
		tmp += it->second;
		return tmp;
	}
	return std::string ("200 OK\r\n");
}