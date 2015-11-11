#include <memory>
#include "interface.hpp"
#include "app_state.hpp"

extern "C" {
std::unique_ptr<iface::GuiInterface> CreateInterface();
}

class GtkGuiInterface : public iface::GuiInterface
{
private:
	std::shared_ptr<app::AppState> m_app_state;

public:
	~GtkGuiInterface();

public:
    bool init(int argc, char *argv[]);
    void register_other_interfaces(const std::vector<std::string> &interfaces);
	void register_app_state(std::shared_ptr<app::AppState> state);
	
    int run();
};
