#ifndef __APP_STATE_H__
#define __APP_STATE_H__

#include <string>
#include <map>
#include <vector>


namespace app
{


class AppState
{
private:
	std::map<std::string, std::string> _params;
    std::vector<std::string> _files;

public:
	AppState();
	~AppState();
	
public:
    void init(int argc, char *argv[]);

	std::string get_param(std::string name);
	void set_param(std::string name, std::string value);
    
    std::vector<std::string>& get_file_list();
};


};


#endif
