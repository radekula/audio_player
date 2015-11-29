#ifndef __APP_STATE_H__
#define __APP_STATE_H__

#include <string>
#include <map>


namespace app
{


class AppState
{
private:
	std::map<std::string, std::string> m_params;

public:
	AppState();
	~AppState();
	
public:
	std::string get_param(std::string name);
	void set_param(std::string name, std::string value);
};


};


#endif
