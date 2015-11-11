#include <iostream>
#include <string>
#include "gui_gtk.hpp"

extern "C" {
std::unique_ptr<iface::GuiInterface> CreateInterface()
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;

	std::unique_ptr<iface::GuiInterface> ptr(static_cast<iface::GuiInterface *> (new GtkGuiInterface()));
	
	return ptr;
};
};




GtkGuiInterface::~GtkGuiInterface()
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
};



bool GtkGuiInterface::init(int argc, char *argv[])
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	return false;
};



void GtkGuiInterface::register_other_interfaces(const std::vector<std::string> &interfaces)
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
};



int GtkGuiInterface::run()
{
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	return 0;
};



void GtkGuiInterface::register_app_state(std::shared_ptr<app::AppState> state)
{
	m_app_state = state;
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
};
