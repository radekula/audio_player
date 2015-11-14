#include "interface.hpp"
#include "app_state.hpp"

extern "C" {
std::unique_ptr<iface::GuiInterface> CreateInterface();
};


class TermGuiInterface : public iface::GuiInterface
{
private:
	std::shared_ptr<app::AppState> m_app_state;

public:
	~TermGuiInterface();
	
public:
    std::string interface_info();
    
public:
    bool init(int argc, char *argv[]);
    void register_other_interfaces(const std::vector<std::string> &interfaces);
	void register_app_state(std::shared_ptr<app::AppState> state);

    void run();
};
