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


int refresh_signal(void *interface)
{
	((GtkGuiInterface *) interface)->refresh();

	return 1;
};


GtkGuiInterface::GtkGuiInterface()
{
	_primary = true;
	
	_was_initiated = false;
	_player_window = nullptr;
	_play_button = nullptr;
	_stop_button = nullptr;
	_pause_button = nullptr;
    _next_button = nullptr;
    _prev_button = nullptr;
    _player_progress_bar = nullptr;
    _curr_playing_label = nullptr;
    _cover = nullptr;
    _switch_button = nullptr;
    
    _player_window_cover = nullptr;
    _play_icon_cover = nullptr;
	_stop_icon_cover = nullptr;
	_pause_icon_cover = nullptr;
    _next_icon_cover = nullptr;
    _prev_icon_cover = nullptr;
    _curr_playing_label_cover = nullptr;
    _cover_cover = nullptr;
    _switch_icon_cover = nullptr;
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
	gtk_window_set_default_size(GTK_WINDOW(_player_window), 100, 100);
	
	g_signal_connect_swapped(G_OBJECT(_player_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkRequisition size;
	size.width = 32;
	size.height = 32;
	
	_prev_button = gtk_button_new_from_icon_name("media-skip-backward", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_prev_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_prev_button), &size, &size);
    g_signal_connect(G_OBJECT(_prev_button), "button-release-event", G_CALLBACK(signal_play_prev), this);

	_play_button = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_play_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_play_button), &size, &size);
    g_signal_connect(G_OBJECT(_play_button), "button-release-event", G_CALLBACK(signal_play), this);
		
	_pause_button = gtk_button_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_pause_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_pause_button), &size, &size);
    g_signal_connect(G_OBJECT(_pause_button), "button-release-event", G_CALLBACK(signal_pause), this);
		
	_stop_button = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_stop_button)
		throw -1;	
	gtk_widget_get_preferred_size(GTK_WIDGET(_stop_button), &size, &size);
    g_signal_connect(G_OBJECT(_stop_button), "button-release-event", G_CALLBACK(signal_stop), this);

	_next_button = gtk_button_new_from_icon_name("media-skip-forward", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_next_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_next_button), &size, &size);
    g_signal_connect(G_OBJECT(_next_button), "button-release-event", G_CALLBACK(signal_play_next), this);

	_play_list_button = gtk_button_new_from_icon_name("edit-select-all", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_play_list_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_play_list_button), &size, &size);
    g_signal_connect(G_OBJECT(_play_list_button), "button-release-event", G_CALLBACK(signal_edit_play_list), this);

	_switch_button = gtk_button_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_switch_button)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_switch_button), &size, &size);
    g_signal_connect(G_OBJECT(_switch_button), "button-release-event", G_CALLBACK(signal_change_view), this);
    	
	GtkWidget *_player_window_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(_player_window_vbox), FALSE);
	gtk_container_add(GTK_CONTAINER(_player_window), GTK_WIDGET(_player_window_vbox));
	
	_cover = gtk_image_new();
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(_cover), TRUE, TRUE, 0);
	
	GtkWidget *_player_button_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(_player_button_hbox), FALSE);
	
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_prev_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_play_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_pause_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_stop_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_next_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_play_list_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_player_button_hbox), GTK_WIDGET(_switch_button), FALSE, FALSE, 0);
	
    _curr_playing_label = gtk_label_new("No file selected");
	gtk_widget_set_halign(GTK_WIDGET(_curr_playing_label), GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(_curr_playing_label), FALSE, TRUE, 0);	
    
	GtkWidget *progress_bar_event_box = gtk_event_box_new();
	g_signal_connect(G_OBJECT(progress_bar_event_box), "button-press-event", G_CALLBACK(signal_progress_on_mouse_click), this);
	
	_player_progress_bar = gtk_progress_bar_new();
	gtk_widget_set_valign(GTK_WIDGET(_player_progress_bar), GTK_ALIGN_CENTER);	
	
	gtk_container_add(GTK_CONTAINER(progress_bar_event_box), _player_progress_bar);
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(progress_bar_event_box), FALSE, TRUE, 0);		
	gtk_widget_set_halign(GTK_WIDGET(_player_button_hbox), GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(_player_window_vbox), GTK_WIDGET(_player_button_hbox), FALSE, TRUE, 0);
	
/////////////////////////

	_player_window_cover = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	if(!_player_window_cover)
		throw -1;

	GtkWidget *event_box = gtk_event_box_new();
	gtk_widget_add_events(_player_window_cover, GDK_LEAVE_NOTIFY_MASK);
	gtk_widget_add_events(_player_window_cover, GDK_ENTER_NOTIFY_MASK);
	g_signal_connect(G_OBJECT(_player_window_cover), "leave-notify-event", G_CALLBACK(signal_cover_on_mouse_leave), this);
	g_signal_connect(G_OBJECT(_player_window_cover), "enter-notify-event", G_CALLBACK(signal_cover_on_mouse_enter), this);
	
	gtk_window_set_title(GTK_WINDOW(_player_window_cover), "Audio player");
	gtk_window_set_resizable(GTK_WINDOW(_player_window_cover), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(_player_window_cover), 300, 300);
	gtk_window_set_decorated(GTK_WINDOW(_player_window_cover), false);
	
	g_signal_connect_swapped(G_OBJECT(_player_window_cover), "destroy", G_CALLBACK(gtk_main_quit), this);
	
	size.width = 64;
	size.height = 64;

// overlay
	GtkWidget *_overlay = gtk_overlay_new();
	gtk_container_add(GTK_CONTAINER(_player_window_cover), GTK_WIDGET(_overlay));

// background image (cover)
	_cover_cover = gtk_image_new();	
	gtk_container_add(GTK_CONTAINER(_overlay), GTK_WIDGET(_cover_cover));
	
// vbox (player window on top of cover)
	GtkWidget *_overlay_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(_overlay_vbox), FALSE);	

	_buttons_cover_revealer = gtk_revealer_new();
	gtk_revealer_set_transition_duration(GTK_REVEALER(_buttons_cover_revealer), 1000); 
	gtk_revealer_set_transition_type(GTK_REVEALER(_buttons_cover_revealer), GTK_REVEALER_TRANSITION_TYPE_CROSSFADE); 
	gtk_container_add(GTK_CONTAINER(_buttons_cover_revealer), _overlay_vbox);
	gtk_overlay_add_overlay(GTK_OVERLAY(_overlay), GTK_WIDGET(_buttons_cover_revealer));
	
// empty label to fill space
	gtk_box_pack_start(GTK_BOX(_overlay_vbox), gtk_label_new(""), TRUE, TRUE, 0);	
	
// controls
	GtkWidget *_player_button_cover_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_set_homogeneous(GTK_BOX(_player_button_cover_hbox), FALSE);
	gtk_box_pack_start(GTK_BOX(_overlay_vbox), GTK_WIDGET(_player_button_cover_hbox), FALSE, TRUE, 0);

	GtkWidget *prev_icon_event_box = gtk_event_box_new();
	_prev_icon_cover = gtk_image_new_from_icon_name("media-skip-backward", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_prev_icon_cover)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_prev_icon_cover), &size, &size);
    g_signal_connect(G_OBJECT(prev_icon_event_box), "button-release-event", G_CALLBACK(signal_play_prev), this);
  	gtk_container_add(GTK_CONTAINER(prev_icon_event_box), GTK_WIDGET(_prev_icon_cover));
	gtk_box_pack_start(GTK_BOX(_player_button_cover_hbox), GTK_WIDGET(prev_icon_event_box), TRUE, FALSE, 0);
	
	GtkWidget *play_icon_event_box = gtk_event_box_new();
	_play_icon_cover = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_play_icon_cover)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_play_icon_cover), &size, &size);
    g_signal_connect(G_OBJECT(play_icon_event_box), "button-release-event", G_CALLBACK(signal_play), this);
  	gtk_container_add(GTK_CONTAINER(play_icon_event_box), GTK_WIDGET(_play_icon_cover));
	gtk_box_pack_start(GTK_BOX(_player_button_cover_hbox), GTK_WIDGET(play_icon_event_box), TRUE, FALSE, 0);
		
	GtkWidget *pause_icon_event_box = gtk_event_box_new();
	_pause_icon_cover = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_pause_icon_cover)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_pause_icon_cover), &size, &size);
    g_signal_connect(G_OBJECT(pause_icon_event_box), "button-release-event", G_CALLBACK(signal_pause), this);
  	gtk_container_add(GTK_CONTAINER(pause_icon_event_box), GTK_WIDGET(_pause_icon_cover));
	gtk_box_pack_start(GTK_BOX(_player_button_cover_hbox), GTK_WIDGET(pause_icon_event_box), TRUE, FALSE, 0);
		
	GtkWidget *stop_icon_event_box = gtk_event_box_new();
	_stop_icon_cover = gtk_image_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_stop_icon_cover)
		throw -1;	
	gtk_widget_get_preferred_size(GTK_WIDGET(_stop_icon_cover), &size, &size);
    g_signal_connect(G_OBJECT(stop_icon_event_box), "button-release-event", G_CALLBACK(signal_stop), this);
  	gtk_container_add(GTK_CONTAINER(stop_icon_event_box), GTK_WIDGET(_stop_icon_cover));
	gtk_box_pack_start(GTK_BOX(_player_button_cover_hbox), GTK_WIDGET(stop_icon_event_box), TRUE, FALSE, 0);

	GtkWidget *next_icon_event_box = gtk_event_box_new();
	_next_icon_cover = gtk_image_new_from_icon_name("media-skip-forward", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_next_icon_cover)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_next_icon_cover), &size, &size);
    g_signal_connect(G_OBJECT(next_icon_event_box), "button-release-event", G_CALLBACK(signal_play_next), this);
  	gtk_container_add(GTK_CONTAINER(next_icon_event_box), GTK_WIDGET(_next_icon_cover));
	gtk_box_pack_start(GTK_BOX(_player_button_cover_hbox), GTK_WIDGET(next_icon_event_box), TRUE, FALSE, 0);

	GtkWidget *play_list_icon_event_box = gtk_event_box_new();
	_play_list_icon_cover = gtk_image_new_from_icon_name("edit-select-all", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_play_list_icon_cover)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_play_list_icon_cover), &size, &size);
    g_signal_connect(G_OBJECT(play_list_icon_event_box), "button-release-event", G_CALLBACK(signal_edit_play_list), this);
	gtk_container_add(GTK_CONTAINER(play_list_icon_event_box), GTK_WIDGET(_play_list_icon_cover));
	gtk_box_pack_start(GTK_BOX(_player_button_cover_hbox), GTK_WIDGET(play_list_icon_event_box), TRUE, FALSE, 0);

	GtkWidget *_switch_event_box = gtk_event_box_new();
	_switch_icon_cover = gtk_image_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if(!_switch_icon_cover)
		throw -1;
	gtk_widget_get_preferred_size(GTK_WIDGET(_switch_icon_cover), &size, &size);
    g_signal_connect(G_OBJECT(_switch_event_box), "button-release-event", G_CALLBACK(signal_change_view), this);
	gtk_container_add(GTK_CONTAINER(_switch_event_box), GTK_WIDGET(_switch_icon_cover));
	gtk_box_pack_start(GTK_BOX(_player_button_cover_hbox), GTK_WIDGET(_switch_event_box), TRUE, FALSE, 0);
	
// song title label
    _curr_playing_label_cover = gtk_label_new("No file selected");
	gtk_widget_set_halign(GTK_WIDGET(_curr_playing_label_cover), GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(_overlay_vbox), GTK_WIDGET(_curr_playing_label_cover), FALSE, TRUE, 0);
	
	gtk_revealer_set_reveal_child(GTK_REVEALER(_buttons_cover_revealer), false);
	
	
///////////////////////
	_play_list_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	if(!_play_list_window)
		throw -1;
		
	gtk_window_set_title(GTK_WINDOW(_play_list_window), "Audio player - playlist");
	gtk_window_set_resizable(GTK_WINDOW(_play_list_window), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(_play_list_window), 500, 600);
	g_signal_connect(GTK_WIDGET(_play_list_window), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);

    GtkWidget *playlist_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(_play_list_window), GTK_WIDGET(playlist_vbox));
    
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window), GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(playlist_vbox), scrolled_window, TRUE, TRUE, 0);
    
	_list_store = gtk_list_store_new(1, G_TYPE_STRING);

    _treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(_list_store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), _treeview);

    auto renderer = gtk_cell_renderer_text_new();
	auto column = gtk_tree_view_column_new_with_attributes("Nazwa", renderer, "text", 0, NULL);
	gtk_tree_view_column_set_sort_column_id(column, 0);
	gtk_tree_view_append_column(GTK_TREE_VIEW(_treeview), column);
    
	GtkWidget *play_list_button_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_pack_start(GTK_BOX(playlist_vbox), GTK_WIDGET(play_list_button_hbox), FALSE, TRUE, 0);
    
	GtkWidget *button = gtk_button_new_with_label("Dodaj");
	if(!button)
		throw -1;
    g_signal_connect(G_OBJECT(button), "button-release-event", G_CALLBACK(signal_add_to_play_list), this);  
    gtk_box_pack_start(GTK_BOX(play_list_button_hbox), GTK_WIDGET(button), TRUE, TRUE, 0);

	button = gtk_button_new_with_label("Usuń");
	if(!button)
		throw -1;
    g_signal_connect(G_OBJECT(button), "button-release-event", G_CALLBACK(signal_remove_from_play_list), this);  
    gtk_box_pack_start(GTK_BOX(play_list_button_hbox), GTK_WIDGET(button), TRUE, TRUE, 0);
    
    gtk_widget_hide(GTK_WIDGET(_play_list_window));
    
    g_timeout_add(250, refresh_signal, (gpointer) this);
};




void GtkGuiInterface::restore_state()
{
    auto files = _audio_player->get_play_list();
    
    if(files.size() == 0)
    {
        gtk_label_set_label(GTK_LABEL(_curr_playing_label), "No file selected");
        gtk_label_set_label(GTK_LABEL(_curr_playing_label_cover), "No file selected");
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(_curr_playing_label), files.begin()->c_str());
        gtk_label_set_label(GTK_LABEL(_curr_playing_label_cover), files.begin()->c_str());
       
        std::string cover_file = files.begin()->substr(0, files.begin()->size() - 3) + "png";
    
		if(_cover)
		{
			auto pixbuf = gdk_pixbuf_new_from_file(cover_file.c_str(), NULL);
			gtk_image_set_from_pixbuf(GTK_IMAGE(_cover), pixbuf);
			gtk_image_set_from_pixbuf(GTK_IMAGE(_cover_cover), gdk_pixbuf_scale_simple(pixbuf, 300, 300, GDK_INTERP_BILINEAR));
		};
		
        _audio_player->set_file(files.begin()->c_str());
    }
    
    GtkTreeIter iter;
    for(auto g : files)
    {
		gtk_list_store_append(_list_store, &iter);
		gtk_list_store_set(_list_store, &iter, 0, g.c_str(), -1);
	};
};




void GtkGuiInterface::show_interface()
{
	gtk_widget_show_all(GTK_WIDGET(_player_window));
};




void GtkGuiInterface::destroy_interface()
{
};



void GtkGuiInterface::signal_play(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
    if(interface)
        interface->play();
};




void GtkGuiInterface::signal_stop(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
    if(interface)
        interface->stop();
};




void GtkGuiInterface::signal_pause(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
    if(interface)
        interface->pause();
};




void GtkGuiInterface::signal_play_next(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
    if(interface)
        interface->play_next();
};




void GtkGuiInterface::signal_play_prev(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
    if(interface)
        interface->play_prev();
};



void GtkGuiInterface::signal_change_view(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
	if(interface)
        interface->change_view();
};




void GtkGuiInterface::signal_edit_play_list(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
	if(interface)
        interface->edit_play_list();
};



void GtkGuiInterface::signal_add_to_play_list(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
	auto file_chooser = gtk_file_chooser_dialog_new("Wybierz plik", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Wybierz", GTK_RESPONSE_ACCEPT, NULL);
	
	auto filter = gtk_file_filter_new();
	gtk_file_filter_add_mime_type(filter, "audio/ogg");
	
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_chooser), filter);
	auto response = gtk_dialog_run(GTK_DIALOG(file_chooser));
	
	if(response == GTK_RESPONSE_ACCEPT)
		interface->add_to_play_list(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser)));
	
	gtk_widget_destroy(file_chooser);
};



void GtkGuiInterface::signal_remove_from_play_list(GtkButton *button, GdkEvent *event, GtkGuiInterface *interface)
{
	auto selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(interface->_treeview));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(selection), GTK_SELECTION_SINGLE);
    
    GtkTreeModel *model;
    
    auto selected = gtk_tree_selection_get_selected_rows(GTK_TREE_SELECTION(selection), &model);
   
    gchar *buff;
    
    GtkTreeIter iter;
    gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter, (GtkTreePath *)(g_list_first(selected)->data)); 
    gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, 0, &buff, -1);

    std::string file(buff);
    interface->remove_file(file);
};


void GtkGuiInterface::signal_progress_on_mouse_click(GtkWidget *widget, GdkEventButton *event, GtkGuiInterface *interface)
{
	double width = gtk_widget_get_allocated_width(widget);
	
	double percent = (double) (event->x * 100) / width;
	interface->set_pos(percent);	
};


void GtkGuiInterface::signal_cover_on_mouse_leave(GtkWidget *widget, GdkEvent *event, GtkGuiInterface *interface)
{
	interface->hide_controls();
};



void GtkGuiInterface::signal_cover_on_mouse_enter(GtkWidget *widget, GdkEvent *event, GtkGuiInterface *interface)
{
	interface->show_controls();
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
    gtk_label_set_label(GTK_LABEL(_curr_playing_label_cover), _audio_player->get_curr_file().c_str());
    
    std::string cover_file = _audio_player->get_curr_file().substr(0, _audio_player->get_curr_file().size() - 3) + "png";
    
	if(_cover)
	{
		auto pixbuf = gdk_pixbuf_new_from_file(cover_file.c_str(), NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE(_cover), pixbuf);
		gtk_image_set_from_pixbuf(GTK_IMAGE(_cover_cover), gdk_pixbuf_scale_simple(pixbuf, 300, 300, GDK_INTERP_BILINEAR));
	};
};




void GtkGuiInterface::play_prev()
{
    _audio_player->prev();

    gtk_label_set_label(GTK_LABEL(_curr_playing_label), _audio_player->get_curr_file().c_str());
    gtk_label_set_label(GTK_LABEL(_curr_playing_label_cover), _audio_player->get_curr_file().c_str());
    
    std::string cover_file = _audio_player->get_curr_file().substr(0, _audio_player->get_curr_file().size() - 3) + "png";
    
	if(_cover)
	{
		auto pixbuf = gdk_pixbuf_new_from_file(cover_file.c_str(), NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE(_cover), pixbuf);
		gtk_image_set_from_pixbuf(GTK_IMAGE(_cover_cover), gdk_pixbuf_scale_simple(pixbuf, 300, 300, GDK_INTERP_BILINEAR));
	};
};



void GtkGuiInterface::change_view()
{
	if(_primary)
	{
		gtk_widget_hide(GTK_WIDGET(_player_window));
		gtk_widget_show_all(GTK_WIDGET(_player_window_cover));
		gtk_revealer_set_reveal_child(GTK_REVEALER(_buttons_cover_revealer), false);
		gtk_window_set_keep_above(GTK_WINDOW(_player_window_cover), true);
		_primary = false;
	}
	else
	{
		gtk_window_set_keep_above(GTK_WINDOW(_player_window_cover), false);
		gtk_revealer_set_reveal_child(GTK_REVEALER(_buttons_cover_revealer), false);
		gtk_widget_hide(GTK_WIDGET(_player_window_cover));
		gtk_widget_show_all(GTK_WIDGET(_player_window));
		gtk_widget_show_all(GTK_WIDGET(_player_window));
		_primary = true;
	};
};



void GtkGuiInterface::edit_play_list()
{
	gtk_widget_show_all(_play_list_window);	
};



void GtkGuiInterface::remove_file(std::string file)
{
    _audio_player->remove_from_play_list(file);
    
    GtkTreeIter iter;
    gchar *buff;
    
    auto model = gtk_tree_view_get_model(GTK_TREE_VIEW(_treeview));
    auto is_row = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(model), &iter);
    
    while(is_row)
    {
        gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, 0, &buff, -1);
        
        if(file == std::string(buff))
        {
            is_row = false;
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
        };
        
        is_row = gtk_tree_model_iter_next(GTK_TREE_MODEL(model), &iter);
    };
    
    gtk_label_set_label(GTK_LABEL(_curr_playing_label), _audio_player->get_curr_file().c_str());
    gtk_label_set_label(GTK_LABEL(_curr_playing_label_cover), _audio_player->get_curr_file().c_str());
    
    std::string cover_file = _audio_player->get_curr_file().substr(0, _audio_player->get_curr_file().size() - 3) + "png";
    
	if(_cover)
	{
		auto pixbuf = gdk_pixbuf_new_from_file(cover_file.c_str(), NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE(_cover), pixbuf);
		gtk_image_set_from_pixbuf(GTK_IMAGE(_cover_cover), gdk_pixbuf_scale_simple(pixbuf, 300, 300, GDK_INTERP_BILINEAR));
	};
};




void GtkGuiInterface::add_to_play_list(std::string file)
{
    GtkTreeIter iter;

	gtk_list_store_append(_list_store, &iter);
	gtk_list_store_set(_list_store, &iter, 0, file.c_str(), -1);
	
	_audio_player->add_to_play_list(file);
};


void GtkGuiInterface::show_controls()
{
	gtk_revealer_set_reveal_child(GTK_REVEALER(_buttons_cover_revealer), true);	
};



void GtkGuiInterface::hide_controls()
{
	gtk_revealer_set_reveal_child(GTK_REVEALER(_buttons_cover_revealer), false);	
};



void GtkGuiInterface::refresh()
{
	if(_audio_player)
	{
		double length = _audio_player->get_length();
		double pos = _audio_player->get_position();

		double fraction;
		
		if(length > 0)
			fraction = (float) pos / length;
		else
			fraction = 0;
			
			
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(_player_progress_bar), fraction);
	};
};


void GtkGuiInterface::set_pos(double percentage)
{
	if(_audio_player)
		_audio_player->seek(percentage);
};
