#include <string>
#include <iostream>
#include "app_state.hpp"


namespace app {

AppState::AppState()
{
    // default params
    _params["app_running"] = "1";
    _params["app_running"] = "1";

    _params["iface_library_dir"] = "./";
    _params["iface_library"] = "libgui_gtk.so";
	_params["iface_next_iface"] = "0";
	_params["iface_exit_status"] = "OK";
};

AppState::~AppState()
{
};

void AppState::init(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {
        std::string param = argv[i];
        
        if(param == "--help")
        {
            std::cout << std::endl;
            std::cout << "Available parameters:" << std::endl;
            std::cout << std::endl;

            std::cout << "\t" << "--interface [libfile]" << "\t\t"
                << "Sets starting interface." << std::endl;
                        
            std::cout << "\t" << "--interfaces_dir [dir]" << "\t\t"
                << "Set directory for searching interfaces." << std::endl;

            std::cout << "\t" << "--help [dir]" << "\t\t\t"
                << "This help." << std::endl;
            
            std::cout << std::endl;

            set_param("app_running", "0");
            break;
        };
        
        if(param == "--interface")
        {
            set_param("iface_library", argv[++i]);
            continue;
        };

        if(param == "--interfaces_dir")
        {
            set_param("iface_library_dir", argv[++i]);
            continue;
        };

        _files.push_back(param);
    }
};

std::string AppState::get_param(std::string name)
{
	return _params[name];
};


void AppState::set_param(std::string name, std::string value)
{
	_params[name] = value;
};


std::vector<std::string>& AppState::get_file_list()
{
    return _files;
};


}
