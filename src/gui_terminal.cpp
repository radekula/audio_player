#include <iostream>
#include <string>
#include "gui_terminal.hpp"

extern "C" {
std::unique_ptr<iface::GuiInterface> CreateInterface()
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	std::unique_ptr<iface::GuiInterface> ptr(static_cast<iface::GuiInterface *> (new TermGuiInterface()));
	
	return ptr;
};
};


TermGuiInterface::~TermGuiInterface()
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
};



bool TermGuiInterface::init(int argc, char *argv[])
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	return false;
};



void TermGuiInterface::register_other_interfaces(const std::vector<std::string> &interfaces)
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
};



int TermGuiInterface::run()
{
	if(m_app_state)
	{
		m_app_state->SetParam("iface_next_iface", "1");
		m_app_state->SetParam("iface_exit_status", "1");
	};

	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	return 0;
};




void TermGuiInterface::register_app_state(std::shared_ptr<app::AppState> state)
{
	m_app_state = state;
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
};

