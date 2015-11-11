#include <dirent.h>
#include <dlfcn.h>
#include <iostream>
#include <string>
#include "interface.hpp"



namespace iface
{


std::vector<std::string> list_interface_files()
{
	std::vector<std::string> libraries;
	
	DIR *lib_dir;
	struct dirent *file;

	lib_dir = opendir("./lib");
	
	if(lib_dir != nullptr)
	{
		while(file = readdir(lib_dir))
		{
			std::string file_name(file->d_name);
			std::string base_name("libgui");
			std::string extension(".so");
			
			auto pos_base = file_name.find(base_name);
			auto pos_ext = file_name.rfind(extension);
			
			if(pos_base != std::string::npos && pos_ext != std::string::npos && pos_ext == file_name.length() - extension.length())
				libraries.push_back(file_name);
		};	
	};
	
	return libraries;
};



std::unique_ptr<GuiInterface> load_interface(std::string lib_file)
{
	std::unique_ptr<GuiInterface> new_interface;

	auto file = std::string("lib/") + lib_file;

	auto dlhandle = dlopen(file.c_str(), RTLD_LAZY);
	if(!dlhandle)
	{
		std::cout << "Błąd otwierania pliku: " << lib_file << std::endl;
		return nullptr;
	};
	
	dlerror();
	
	std::unique_ptr<iface::GuiInterface> (*create_interface)();
	
	create_interface = (std::unique_ptr<iface::GuiInterface> (*)()) dlsym(dlhandle, "CreateInterface");
	if(!create_interface)
	{
		std::cout << "Nie znaleziono funkcji 'CreateInterface' w bibliotece: " << lib_file << std::endl;
		return nullptr;
	}
	
	new_interface = create_interface();
	return new_interface;
};


};
