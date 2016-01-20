#ifndef __AUDIO_PLAYER_HPP__
#define __AUDIO_PLAYER_HPP__


#include <gst/gst.h>
#include <glib.h>
#include <string>
#include <list>




namespace player
{



class AudioPlayer
{
private:
	GstElement *playback_pipeline;
	GstElement *source;
	GstElement *demuxer;
	GstElement *decoder;
	GstElement *conv;
	GstElement *sink;
	
	GstBus *message_bus;

    std::list<std::string> _play_list;
    std::string _curr_playing;
    
    bool is_playing;

private:
    static void on_pad_added(GstElement *element, GstPad *pad, gpointer data);

private:
    std::unique_ptr<std::list<std::string>::iterator> find_file(std::string file);

public:
	AudioPlayer();
	~AudioPlayer();
	
public:
	void init(int argc, char *argv[]);
	
	void set_file(std::string file);
    std::string get_curr_file();
    
	void play();
	void pause();
	void stop();
    void next();
    void prev();
    
    void add_to_play_list(std::string file);
    void remove_from_play_list(std::string file);
    
    std::list<std::string>& get_play_list();
};



}

#endif
