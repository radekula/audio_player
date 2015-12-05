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
    void register_other_interfaces(const std::vector<iface::GuiInterfaceInfo> &interfaces);
	void register_app_state(std::shared_ptr<app::AppState> state);
	void register_audio_player(std::shared_ptr<player::AudioPlayer> audio_player);
	
    void run();

/**********************************************************************/

private:
	std::shared_ptr<app::AppState> _app_state;
	bool _was_initiated;
    
private:
	GtkWidget *_player_window;
	
	GtkWidget *_play_button;
	GtkWidget *_stop_button;
	GtkWidget *_pause_button;
	GtkWidget *_next_button;
	GtkWidget *_prev_button;
    
    GtkWidget *_player_progress_bar;
    GtkWidget *_curr_playing_label;
	
private:
	void construct_interface();
	void restore_state();
	void show_interface();
	void destroy_interface();

private:
    std::shared_ptr<player::AudioPlayer> _audio_player;
    
private:
    static void signal_play(GtkButton *button, GtkGuiInterface *interface);
    static void signal_stop(GtkButton *button, GtkGuiInterface *interface);
    static void signal_pause(GtkButton *button, GtkGuiInterface *interface);
    static void signal_play_next(GtkButton *button, GtkGuiInterface *interface);
    static void signal_play_prev(GtkButton *button, GtkGuiInterface *interface);

public:
    void play();
    void stop();
    void pause();
    void play_next();
    void play_prev();
    
public:
	GtkGuiInterface();
};
