#include "app_state.hpp"


namespace app {

AppState::AppState()
{
    // default params
    m_params["interfaces_library_dir"] = "./";

	m_params["iface_next_iface"] = "0";
	m_params["iface_exit_status"] = "OK";
};

AppState::~AppState()
{
};

std::string AppState::get_param(std::string name)
{
	std::string value;
	value = m_params[name];
	return value;
};


void AppState::set_param(std::string name, std::string value)
{
	m_params[name] = value;
};


}
