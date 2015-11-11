#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__


#include <string>
#include <memory>
#include <vector>
#include "app_state.hpp"



namespace iface
{




/*
class GuiWindow
{
public:
    virtual ~GuiWindow() {};
};




class GuiButton
{
public:
    virtual ~GuiButton() {};
};




class GuiLabel
{
public:
    virtual ~GuiLabel() {};
};




class GuiEdit
{
public:
    virtual ~GuiEdit() {};
};
*/



class GuiInterface
{
public:
    virtual ~GuiInterface() {};

public:
    virtual bool init(int argc, char *argv[]) = 0;
    virtual void register_other_interfaces(const std::vector<std::string> &interfaces) = 0;
	virtual void register_app_state(std::shared_ptr<app::AppState> state) = 0;
    
    virtual int run() = 0;
};


std::unique_ptr<GuiInterface> create_interface();

std::vector<std::string> list_interface_files();
std::unique_ptr<GuiInterface> load_interface(std::string lib_file);


}


#endif
