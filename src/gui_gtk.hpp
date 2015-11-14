#include <memory>
#include <gtk/gtk.h>
#include "interface.hpp"
#include "app_state.hpp"




extern "C" {
std::unique_ptr<iface::GuiInterface> CreateInterface();
}




class GtkGuiInterface : public iface::GuiInterface
{
public:
	~GtkGuiInterface();

public:
    std::string interface_info();

public:
    bool init(int argc, char *argv[]);
    void register_other_interfaces(const std::vector<std::string> &interfaces);
	void register_app_state(std::shared_ptr<app::AppState> state);
	
    void run();

/**********************************************************************/

private:
	std::shared_ptr<app::AppState> m_app_state;
	bool was_initiated;
    
private:
	GtkWidget *player_window;
	
	GtkWidget *play_button;
	GtkWidget *stop_button;
	GtkWidget *pause_button;
	
private:
	void construct_interface();
	void restore_state();
	void show_interface();
	void destroy_interface();

public:
	GtkGuiInterface();
};
