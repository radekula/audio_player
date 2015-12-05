#include <iostream>
#include "audio_player.hpp"



namespace player
{





AudioPlayer::AudioPlayer()
{
	playback_pipeline = nullptr;
	source = nullptr;
	demuxer = nullptr;
	decoder = nullptr;
	conv = nullptr;
	sink = nullptr;
	message_bus = nullptr;
};


void AudioPlayer::init(int argc, char *argv[])
{
	gst_init(&argc, &argv);
	
	playback_pipeline = gst_pipeline_new("playback_pipeline");
	source = gst_element_factory_make("filesrc", "file-source");
	demuxer = gst_element_factory_make("oggdemux", "ogg-demuxer");
	decoder = gst_element_factory_make("vorbisdec", "vorbis-decoder");
	conv = gst_element_factory_make("audioconvert", "converter");
	sink = gst_element_factory_make("autoaudiosink", "audio-output");
	
	message_bus = gst_pipeline_get_bus(GST_PIPELINE(playback_pipeline));
//	bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
	
	gst_object_unref(message_bus);
	gst_bin_add_many(GST_BIN(playback_pipeline), source, demuxer, decoder, conv, sink, NULL);
	  
	gst_element_link(source, demuxer);
	gst_element_link_many (decoder, conv, sink, NULL);

};


AudioPlayer::~AudioPlayer()
{
	if(playback_pipeline)
	{
		gst_element_set_state(playback_pipeline, GST_STATE_NULL);
		gst_object_unref(playback_pipeline);
//		g_source_remove(bus_watch_id);
	};
};



void AudioPlayer::set_file(std::string file)
{
    for(auto iter = _play_list.begin(); iter != _play_list.end(); iter++)
    {
        if(*iter == file)
            _curr_playing = iter;
    }
    
	if(source)
		g_object_set(G_OBJECT(source), "location", _curr_playing->c_str(), NULL);
	else
		throw -1;
};




std::string AudioPlayer::get_curr_file()
{
    return *_curr_playing;
};



void AudioPlayer::play()
{
	if(playback_pipeline)
		gst_element_set_state(GST_ELEMENT(playback_pipeline), GST_STATE_PLAYING);
};




void AudioPlayer::pause()
{
};




void AudioPlayer::stop()
{
};



void AudioPlayer::next()
{
    _curr_playing++;

    if(_curr_playing == _play_list.end())
        _curr_playing--;
};



void AudioPlayer::prev()
{
    if(_curr_playing == _play_list.begin())
        return;
        
    _curr_playing--;
};



void AudioPlayer::add_to_play_list(std::string file)
{
    _play_list.push_back(file);
};




void AudioPlayer::remove_from_play_list(std::string file)
{
    for(auto iter = _play_list.begin(); iter != _play_list.end(); iter++)
    {
        if(*iter == file)
            _play_list.erase(iter);
    };
};




std::list<std::string>& AudioPlayer::get_play_list()
{
    return _play_list;
};


};
