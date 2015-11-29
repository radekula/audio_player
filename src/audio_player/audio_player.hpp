#ifndef __AUDIO_PLAYER_HPP__
#define __AUDIO_PLAYER_HPP__


#include <gst/gst.h>
#include <glib.h>
#include <string>




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

	
public:
	AudioPlayer();
	~AudioPlayer();
	
public:
	void init(int argc, char *argv[]);
	
	void set_file(std::string file_path);
	void play();
	void pause();
	void stop();
};



}

#endif
