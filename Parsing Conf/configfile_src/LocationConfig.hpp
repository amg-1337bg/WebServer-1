/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamghoug <bamghoug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 00:32:50 by mbrija            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/06/04 13:06:00 by bamghoug         ###   ########.fr       */
=======
/*   Updated: 2022/06/04 13:36:29 by mbrija           ###   ########.fr       */
>>>>>>> 7c42e3b58ffe4cda8da9551ba15a40dcee0240ca
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "../global_src/support_funcs.hpp"

class LocationConfig
{
private:
    std::string name;
    std::string location_index;
    std::string root;
    std::vector<std::string> redirect;
    std::string upload;
    std::vector<std::string> methods;
    std::vector<std::string> cgi;
    bool auto_index;
    
public:
    LocationConfig(/* args */);
    LocationConfig(LocationConfig const & p);	
    ~LocationConfig();
    LocationConfig & operator= (LocationConfig const &p);
    //getters
    std::string get_name();
    std::string get_location_index();
    std::string get_root();
    std::vector<std::string> get_redirect();
    std::string get_upload();
    std::vector<std::string> getMethods();
    std::vector<std::string> getCgi();
	bool	get_autoindex();
    
       
<<<<<<< HEAD
// friend std::ostream &operator<<(std::ostream &output, LocationConfig &D ) 
// { 
//         output << "name : " << D.get_name() << std::endl;
//         output << "location_index : " << D.get_location_index() << std::endl;
//         output << "root : " << D.get_root() << std::endl;
//         output << "redirect : " << D.get_redirect() << std::endl;
//         output << "upload : " << D.get_upload() << std::endl;
//         for (auto i: D.getMethods())
//             std::cout << "Mathods " << i << ' ' << std::endl;
//         for (auto i: D.getCgi())
//             std::cout << "Cgi " << i << ' ' << std::endl;
//         return output;
// }
=======
friend std::ostream &operator<<(std::ostream &output, LocationConfig &D ) 
{ 
        output << "name : " << D.get_name() << std::endl;
        output << "location_index : " << D.get_location_index() << std::endl;
        output << "root : " << D.get_root() << std::endl;
        output << "upload : " << D.get_upload() << std::endl;
        // for (auto i: D.get_redirect())
        // {
        //     output << "redirect : " << i << std::endl;
        // }
        std::vector<std::string> redir = D.get_redirect();
        if (!redir.empty())
        {
            output << "redirect code : " << redir[0] << std::endl;
            output << "redirect link : " << redir[1] << std::endl;
        }
        for (auto i: D.getMethods())
            std::cout << "Mathods " << i << ' ' << std::endl;
        for (auto i: D.getCgi())
            std::cout << "Cgi " << i << ' ' << std::endl;
        return output;
}
>>>>>>> 7c42e3b58ffe4cda8da9551ba15a40dcee0240ca

    //parse here
    void locationParser(std::string buf);

    class Error_exc : public std::exception {
    private:
        std::string error_msg;
    public:
        explicit Error_exc( const std::string &error_msg ) throw() : error_msg(error_msg) {}
        ~Error_exc() throw() {}

        virtual const char * what() const throw() {
            return (error_msg.c_str());
        }
    };
};

