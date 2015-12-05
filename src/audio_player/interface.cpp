#include <dirent.h>
#include <dlfcn.h>
#include <iostream>
#include <string>
#include "interface.hpp"



namespace iface
{


std::vector<GuiInterfaceInfo> list_interface_files(std::string interfaces_library_dir)
{
	std::vector<GuiInterfaceInfo> libraries;
	
	DIR *lib_dir;
	struct dirent *file;

	lib_dir = opendir(interfaces_library_dir.c_str());
	
	if(lib_dir != nullptr)
	{
		while(file = readdir(lib_dir))
		{
            GuiInterfaceInfo info;
            
			std::string file_name(file->d_name);
			std::string base_name("libgui");
			std::string extension(".so");
			
			auto pos_base = file_name.find(base_name);
			auto pos_ext = file_name.rfind(extension);
			
            info.name = file_name;
            info.file_path = interfaces_library_dir + "/" + file_name;
            
			if(pos_base != std::string::npos && pos_ext != std::string::npos && pos_ext == file_name.length() - extension.length())
				libraries.push_back(info);
		};	
	};
	
	return libraries;
};



std::unique_ptr<GuiInterface> load_interface(std::string lib_file)
{
	std::unique_ptr<GuiInterface> new_interface;

	auto file = lib_file;

	auto dlhandle = dlopen(file.c_str(), RTLD_LAZY);
	if(!dlhandle)
	{
		std::cout << "Error operning file: " << lib_file << std::endl;
		return nullptr;
	};
	
	dlerror();
	
	std::unique_ptr<iface::GuiInterface> (*create_interface)();
	
	create_interface = (std::unique_ptr<iface::GuiInterface> (*)()) dlsym(dlhandle, "CreateInterface");
	if(!create_interface)
	{
		std::cout << "Cannot find 'CreateInterface' function in library: " << lib_file << std::endl;
		return nullptr;
	}
	
	new_interface = create_interface();
	return new_interface;
};


};
