#include "app_state.hpp"


namespace app {

AppState::AppState()
{
	m_params["iface_next_iface"] = "0";
	m_params["iface_exit_status"] = "OK";
};

AppState::~AppState()
{
};

std::string AppState::GetParam(std::string name)
{
	std::string value;
	value = m_params[name];
	return value;
};


void AppState::SetParam(std::string name, std::string value)
{
	m_params[name] = value;
};


}
