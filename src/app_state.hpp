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
	std::string GetParam(std::string name);
	void SetParam(std::string name, std::string value);
};


};


#endif
