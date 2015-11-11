#include <iostream>
#include <memory>
#include "interface.hpp"
#include "app_state.hpp"




int main(int argc, char *argv[])
{
	std::shared_ptr<app::AppState> app_state(new app::AppState());
    std::vector<std::string> interfaces_list = iface::list_interface_files();
	
	if(interfaces_list.size() == 0)
	{
		std::cout << "Nie znaleziono biliotek interface'u" << std::endl;
		return 1;
	};

	for(auto iter : interfaces_list)
		std::cout << "Znaleziono bibilotekę interface'u: " << iter << std::endl;

    auto active_interface = iface::load_interface(interfaces_list[0]);

	if(!active_interface)
	{
		std::cout << "Nie udało się załadować interface'u: " << interfaces_list[0] << std::endl;
		return 1;
	};
	
    while(1)
    {
		active_interface->init(argc, argv);
		active_interface->register_other_interfaces(interfaces_list);
		active_interface->register_app_state(app_state);
		active_interface->run();

		auto status = std::atoi(app_state->GetParam("iface_exit_status").c_str());
		
		switch(status)
		{
			case 0:
			{
				return 0;
			}
			case 1:
			{
				auto iface_num = std::atoi(app_state->GetParam("iface_next_iface").c_str());
				
				active_interface = iface::load_interface(interfaces_list[iface_num]);
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
