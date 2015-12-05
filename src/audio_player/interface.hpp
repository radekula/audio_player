#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__


#include <string>
#include <memory>
#include <vector>
#include "app_state.hpp"
#include "audio_player.hpp"



namespace iface
{


struct GuiInterfaceInfo
{
    std::string name;
    std::string file_path;
};


class GuiInterface
{
public:
    virtual ~GuiInterface() {};

public:
    virtual std::string interface_info() = 0;

public:
    virtual bool init(int argc, char *argv[]) = 0;
    virtual void register_other_interfaces(const std::vector<GuiInterfaceInfo> &interfaces) = 0;
	virtual void register_app_state(std::shared_ptr<app::AppState> state) = 0;
	virtual void register_audio_player(std::shared_ptr<player::AudioPlayer> audio_player) = 0;
    
    virtual void run() = 0;
};


std::unique_ptr<GuiInterface> create_interface();

std::vector<GuiInterfaceInfo> list_interface_files(std::string interfaces_library_dir);
std::unique_ptr<GuiInterface> load_interface(std::string lib_file);


}


#endif
