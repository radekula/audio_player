#include <iostream>
#include <string>
#include "gui_gtk.hpp"

extern "C" {
std::unique_ptr<iface::GuiInterface> CreateInterface()
{
	std::unique_ptr<iface::GuiInterface> ptr(static_cast<iface::GuiInterface *> (new GtkGuiInterface()));
	
	return ptr;
};
};




GtkGuiInterface::GtkGuiInterface()
{
	_was_initiated = false;
	_player_window = nullptr;
	_play_button = nullptr;
	_stop_button = nullptr;
	_pause_button = nullptr;
    _next_button = nullptr;
    _prev_button = nullptr;
    _player_progress_bar = nullptr;
    _curr_playing_label = nullptr;
};




GtkGuiInterface::~GtkGuiInterface()
{
};



std::string GtkGuiInterface::interface_info()
{
	std::string info("Interface based on GTK3 (graphical)");

	return info;
};




bool GtkGuiInterface::init(int argc, char *argv[])
{
	if(_was_initiated)
		return true;
	
	return _was_initiated = (bool) gtk_init_check(&argc, &argv);
};



void GtkGuiInterface::register_other_interfaces(const std::vector<iface::GuiInterfaceInfo> &interfaces)
{
	std::cout << "TODO: " << __FILE__ << ": " << __LINE__ << std::endl;
};



void GtkGuiInterface::run()
{
	try
	{
		if(_was_initiated)
		{
			construct_interface();
			restore_state();
			show_interface();
		
			gtk_main();
			
			destroy_interface();
		};
	}
	catch(...)
	{
	};
};



void GtkGuiInterface::register_app_state(std::shared_ptr<app::AppState> state)
{
	_app_state = state;
};




void GtkGuiInterface::register_audio_player(std::shared_ptr<player::AudioPlayer> audio_player)
{
	_audio_player = audio_player;
};




void GtkGuiInterface::construct_interface()
{
	_player_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	if(!_player_window)
		throw -1;
	
	gtk_window_set_title(GTK_WINDOW(_player_window), "Audio player");
	gtk_window_set_resizable(GTK_WINDOW(_player_window), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(_player_window), 400, 200);
	
	g_signal_connect_swapped(G_OBJECT(_player_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	
	GtkRequisition size;
	size.width = 32;
	size.height = 32;
	
	_prev_button = gtk_button_new_from_icon_name("media-skip-backward", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_prev_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_prev_button), &size, &size);
    g_signal_connect(G_OBJECT(_prev_button), "clicked", G_CALLBACK(signal_play_prev), this);

	_play_button = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_play_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_play_button), &size, &size);
    g_signal_connect(G_OBJECT(_play_button), "clicked", G_CALLBACK(signal_play), this);
		
	_pause_button = gtk_button_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_pause_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_pause_button), &size, &size);
    g_signal_connect(G_OBJECT(_pause_button), "clicked", G_CALLBACK(signal_pause), this);
		
	_stop_button = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_stop_button)
		throw -1;	
	gtk_widget_get_preferred_size(GTK_WIDGET(_stop_button), &size, &size);
    g_signal_connect(G_OBJECT(_stop_button), "clicked", G_CALLBACK(signal_stop), this);

	_next_button = gtk_button_new_from_icon_name("media-skip-forward", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_next_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_next_button), &size, &size);
    g_signal_connect(G_OBJECT(_next_button), "clicked", G_CALLBACK(signal_play_next), this);
	
	GtkWidget *_player_window_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(_player_window_vbox), FALSE);
	gtk_container_add(GTK_CONTAINER(_player_window), GTK_WIDGET(_player_window_vbox));
	
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(gtk_label_new("audio_player")), TRUE, TRUE, 0);

	
	GtkWidget *_player_button_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(_player_button_hbox), FALSE);
	
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_prev_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_play_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_pause_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_stop_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_next_button), FALSE, FALSE, 0);
	
    _curr_playing_label = gtk_label_new("No file selected");
	gtk_widget_set_halign(GTK_WIDGET(_curr_playing_label), GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(_curr_playing_label), FALSE, TRUE, 0);	
    
	_player_progress_bar = gtk_progress_bar_new();
	gtk_widget_set_valign(GTK_WIDGET(_player_progress_bar), GTK_ALIGN_CENTER);
	
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(_player_progress_bar), FALSE, TRUE, 0);		
	gtk_widget_set_halign(GTK_WIDGET(_player_button_hbox), GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(_player_button_hbox), FALSE, TRUE, 0);	
};




void GtkGuiInterface::restore_state()
{
    auto files = _audio_player->get_play_list();

    if(files.size() == 0)
    {
        gtk_label_set_label(GTK_LABEL(_curr_playing_label), "No file selected");
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(_curr_playing_label), files.begin()->c_str());
        _audio_player->set_file(files.begin()->c_str());
    }
};




void GtkGuiInterface::show_interface()
{
	gtk_widget_show_all(GTK_WIDGET(_player_window));
};




void GtkGuiInterface::destroy_interface()
{
};



void GtkGuiInterface::signal_play(GtkButton *button, GtkGuiInterface *interface)
{
    if(interface)
        interface->play();
};




void GtkGuiInterface::signal_stop(GtkButton *button, GtkGuiInterface *interface)
{
    if(interface)
        interface->stop();
};




void GtkGuiInterface::signal_pause(GtkButton *button, GtkGuiInterface *interface)
{
    if(interface)
        interface->pause();
};




void GtkGuiInterface::signal_play_next(GtkButton *button, GtkGuiInterface *interface)
{
    if(interface)
        interface->play_next();
};




void GtkGuiInterface::signal_play_prev(GtkButton *button, GtkGuiInterface *interface)
{
    if(interface)
        interface->play_prev();
};




void GtkGuiInterface::play()
{
    _audio_player->play();
};




void GtkGuiInterface::stop()
{
    _audio_player->stop();
};




void GtkGuiInterface::pause()
{
    _audio_player->pause();
};




void GtkGuiInterface::play_next()
{
    _audio_player->next();

    gtk_label_set_label(GTK_LABEL(_curr_playing_label), _audio_player->get_curr_file().c_str());
};




void GtkGuiInterface::play_prev()
{
    _audio_player->prev();

    gtk_label_set_label(GTK_LABEL(_curr_playing_label), _audio_player->get_curr_file().c_str());
};

