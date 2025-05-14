#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User { 
    public:
        User(string id, string name, string session_token, string primary_image_tag, string device_id, string client_name, string client_version) 
        : id{id}, name{name}, session_token{session_token}, primary_image_tag{primary_image_tag}, device_id{device_id}, client_name{client_name}, client_version{client_version} {
        }

        std::string id;
        std::string name;
        std::string session_token;

        std::string primary_image_tag;
        bool is_admin;
        bool is_logged_in;

        std::string device_id;
        std::string client_name;
        std::string client_version;
    private:
};

#endif