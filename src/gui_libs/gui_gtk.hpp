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
	bool _primary;
    
private:
	GtkWidget *_player_window;
	
	GtkWidget *_play_button;
	GtkWidget *_stop_button;
	GtkWidget *_pause_button;
	GtkWidget *_next_button;
	GtkWidget *_prev_button;
    GtkWidget *_play_list_button;
	GtkWidget *_switch_button;
    GtkWidget *_player_progress_bar;
    GtkWidget *_curr_playing_label;
	GtkWidget *_cover;

	GtkWidget *_buttons_cover_revealer;
	GtkWidget *_player_window_cover;
	GtkWidget *_play_icon_cover;
	GtkWidget *_stop_icon_cover;
	GtkWidget *_pause_icon_cover;
	GtkWidget *_next_icon_cover;
	GtkWidget *_prev_icon_cover;
    GtkWidget *_play_list_icon_cover;
	GtkWidget *_switch_icon_cover;
    GtkWidget *_player_progress_bar_cover;
    GtkWidget *_curr_playing_label_cover;
	GtkWidget *_cover_cover;
	
	GtkWidget *_play_list_window;
	GtkWidget *_treeview;
	GtkListStore *_list_store;
	
private:
	void construct_interface();
	void restore_state();
	void show_interface();
	void destroy_interface();

private:
    std::shared_ptr<player::AudioPlayer> _audio_player;
    
private:
    static void signal_play(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);
    static void signal_stop(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);
    static void signal_pause(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);
    static void signal_play_next(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);
    static void signal_play_prev(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);

    static void signal_change_view(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);
    static void signal_edit_play_list(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);

    static void signal_add_to_play_list(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);
    static void signal_remove_from_play_list(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface);
    
    static void signal_cover_on_mouse_leave(GtkWidget *button, GdkEvent *event, GtkGuiInterface *interface);
    static void signal_cover_on_mouse_enter(GtkWidget *button, GdkEvent *event, GtkGuiInterface *interface);

    static void signal_progress_on_mouse_click(GtkWidget *widget, GdkEventButton *event, GtkGuiInterface *interface);

public:
    void play();
    void stop();
    void pause();
    void play_next();
    void play_prev();

    void change_view();
    void edit_play_list();
    
    void remove_file(std::string file);
    void add_to_play_list(std::string file);
    
    void show_controls();
    void hide_controls();
    
    void refresh();
    void set_pos(double percentage);
    
public:
	GtkGuiInterface();
};
