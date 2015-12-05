#include <iostream>
#include <memory>
#include "interface.hpp"
#include "app_state.hpp"
#include "audio_player.hpp"




int main(int argc, char *argv[])
{
    std::unique_ptr<iface::GuiInterface> active_interface;

    std::shared_ptr<player::AudioPlayer> audio_player = std::make_shared<player::AudioPlayer>();
    audio_player->init(argc, argv);

	std::shared_ptr<app::AppState> app_state(new app::AppState());
    app_state->init(argc, argv);
    
    for(auto file : app_state->get_file_list())
        audio_player->add_to_play_list(file);

    if(app_state->get_param("app_running") == "0")
        return 0;

    std::vector<iface::GuiInterfaceInfo> interfaces_list = iface::list_interface_files(app_state->get_param("iface_library_dir"));
	
	if(interfaces_list.size() == 0)
	{
		std::cout << "Cannot find interface library files" << std::endl;
		return 1;
	};

	for(auto iface_info : interfaces_list)
		std::cout << "Found interface: " << iface_info.name << std::endl;

    std::cout << std::endl << "Using interface: " << app_state->get_param("iface_library") << std::endl;
    
    for(auto iface_info : interfaces_list)
    {
        if(iface_info.name == app_state->get_param("iface_library"))
        {
            active_interface = iface::load_interface(iface_info.file_path);
            break;
        };
    }
    
    if(!active_interface)
    {
        std::cout << "Cannot load interface: " << app_state->get_param("iface_library") << std::endl;
        return 1;
    };
    
    while(app_state->get_param("app_running") != "0")
    {
		active_interface->init(argc, argv);
		active_interface->register_other_interfaces(interfaces_list);
		active_interface->register_app_state(app_state);
        active_interface->register_audio_player(audio_player);
		active_interface->run();

		auto status = std::atoi(app_state->get_param("iface_exit_status").c_str());
		
		switch(status)
		{
			case 0:
			{
				return 0;
			}
			case 1:
			{
				auto iface_num = std::atoi(app_state->get_param("iface_next_iface").c_str());
				
				active_interface = iface::load_interface(interfaces_list[iface_num].file_path);
				break;
			}
			case -1:
			{
				return 1;
			}
			default:
				break;
		};
    };
};
