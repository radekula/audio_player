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
	was_initiated = false;
	player_window = nullptr;
	play_button = nullptr;
	stop_button = nullptr;
	pause_button = nullptr;
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
	if(was_initiated)
		return true;
	
	return was_initiated = (bool) gtk_init_check(&argc, &argv);
};



void GtkGuiInterface::register_other_interfaces(const std::vector<std::string> &interfaces)
{
	std::cout << "TODO: " << __FILE__ << ": " << __LINE__ << std::endl;
};



void GtkGuiInterface::run()
{
	try
	{
		if(was_initiated)
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
	m_app_state = state;
};




void GtkGuiInterface::construct_interface()
{
	player_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	if(!player_window)
		throw -1;
	
	gtk_window_set_title(GTK_WINDOW(player_window), "Audio player");
	gtk_window_set_resizable(GTK_WINDOW(player_window), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(player_window), 400, 200);
	
	g_signal_connect_swapped(G_OBJECT(player_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	
	GtkRequisition size;
	size.width = 32;
	size.height = 32;
	
	play_button = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!play_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(play_button), &size, &size);
		
	pause_button = gtk_button_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!pause_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(pause_button), &size, &size);
		
	stop_button = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!stop_button)
		throw -1;	
	gtk_widget_get_preferred_size(GTK_WIDGET(stop_button), &size, &size);

	
	GtkWidget *player_window_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(player_window_vbox), FALSE);
	gtk_container_add(GTK_CONTAINER(player_window), GTK_WIDGET(player_window_vbox));
	
	gtk_box_pack_start(GTK_BOX(player_window_vbox), GTK_WIDGET(gtk_label_new("audio_player")), TRUE, TRUE, 0);

	
	GtkWidget *player_button_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(player_button_hbox), FALSE);
	
	gtk_box_pack_start(GTK_BOX(player_button_hbox), GTK_WIDGET(play_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(player_button_hbox), GTK_WIDGET(pause_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(player_button_hbox), GTK_WIDGET(stop_button), FALSE, FALSE, 0);
	
	GtkWidget *player_progress_bar = gtk_progress_bar_new();
	gtk_widget_set_valign(GTK_WIDGET(player_progress_bar), GTK_ALIGN_CENTER);
	
	gtk_box_pack_start(GTK_BOX(player_button_hbox), GTK_WIDGET(player_progress_bar), TRUE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(player_window_vbox), GTK_WIDGET(player_button_hbox), FALSE, TRUE, 0);	
};




void GtkGuiInterface::restore_state()
{
};




void GtkGuiInterface::show_interface()
{
	gtk_widget_show_all(GTK_WIDGET(player_window));
};




void GtkGuiInterface::destroy_interface()
{
};
