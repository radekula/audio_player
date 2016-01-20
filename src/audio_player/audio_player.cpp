#include <iostream>
#include <memory>
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



void AudioPlayer::on_pad_added(GstElement *element, GstPad *pad, gpointer data)
{
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    sinkpad = gst_element_get_static_pad(decoder, "sink");

    gst_pad_link(pad, sinkpad);

    gst_object_unref(sinkpad);
};



std::unique_ptr<std::list<std::string>::iterator> AudioPlayer::find_file(std::string file)
{
    std::list<std::string>::iterator iter;
    
    for(iter = _play_list.begin(); iter != _play_list.end(); iter++)
    {
        if(*iter == file)
            break;
    };
    
    std::unique_ptr<std::list<std::string>::iterator> p_iter = std::make_unique<std::list<std::string>::iterator>(iter);
    
    return p_iter;
};



void AudioPlayer::init(int argc, char *argv[])
{
	gst_init(&argc, &argv);

	playback_pipeline = gst_pipeline_new("playback_pipeline");

	source = gst_element_factory_make("filesrc", "file-source");
	if(source == 0)
		std::cout << "Missing source" << std::endl;
		
	demuxer = gst_element_factory_make("oggdemux", "ogg-demuxer");
	if(demuxer == 0)
		std::cout << "Missing demuxer" << std::endl;

	decoder = gst_element_factory_make("vorbisdec", "vorbis-decoder");
	if(decoder == 0)
		std::cout << "Missing decoder" << std::endl;
		
	conv = gst_element_factory_make("audioconvert", "converter");
	if(conv == 0)
		std::cout << "Missing converter" << std::endl;
		
	sink = gst_element_factory_make("autoaudiosink", "audio-output");
	if(sink == 0)
		std::cout << "Missing sink" << std::endl;
			
	message_bus = gst_pipeline_get_bus(GST_PIPELINE(playback_pipeline));
	if(message_bus == 0)
		std::cout << "Missing message bus" << std::endl;
//	bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
	
	gst_object_unref(message_bus);
	gst_bin_add_many(GST_BIN(playback_pipeline), source, demuxer, decoder, conv, sink, NULL);
	  
	gst_element_link(source, demuxer);
	gst_element_link_many (decoder, conv, sink, NULL);
    
    g_signal_connect(demuxer, "pad-added", G_CALLBACK(on_pad_added), decoder);
    
    is_playing = false;
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
    auto p_iter = find_file(file);
    
    if(!p_iter)
        return;
    
    _curr_playing = *(*p_iter);
    
	bool resume = is_playing;
	
	if(resume)
		stop();
		
	if(source)
		g_object_set(G_OBJECT(source), "location", _curr_playing.c_str(), NULL);
	else
		throw -1;
		
	if(resume)
		play();
};




std::string AudioPlayer::get_curr_file()
{
    return _curr_playing;
};



void AudioPlayer::play()
{
	if(playback_pipeline)
	{
		gst_element_set_state(GST_ELEMENT(playback_pipeline), GST_STATE_PLAYING);
		is_playing = true;
	};
};




void AudioPlayer::pause()
{
    if(playback_pipeline)
        gst_element_set_state(GST_ELEMENT(playback_pipeline), GST_STATE_PAUSED);
};




void AudioPlayer::stop()
{
    if(playback_pipeline)
    {
        gst_element_set_state(GST_ELEMENT(playback_pipeline), GST_STATE_READY);
        is_playing = false;
    };
};



void AudioPlayer::next()
{
    auto p_iter = find_file(_curr_playing);
    
    std::list<std::string>::iterator iter;
    
    if(p_iter)
    {
        iter = *p_iter;
        
        iter++;
        if(iter == _play_list.end())
            iter--;
    }
    else
        iter = _play_list.begin();
    
    set_file(*iter);
};



void AudioPlayer::prev()
{
    auto p_iter = find_file(_curr_playing);
    
    std::list<std::string>::iterator iter;
    
    if(p_iter)
    {
        iter = *p_iter;
        
        if(iter != _play_list.begin())
            iter--;
    }
    else
        iter = _play_list.begin();
    
    set_file(*iter);
};



void AudioPlayer::add_to_play_list(std::string file)
{
    _play_list.push_back(file);
};




void AudioPlayer::remove_from_play_list(std::string file)
{
    auto p_iter = find_file(file);
    
    if(p_iter)
    {
        std::list<std::string>::iterator iter = *p_iter;
        iter++;
        
        if(*p_iter == _play_list.begin())
            next();
        else
            prev();
            
        _play_list.erase(*p_iter);
    };    
};




std::list<std::string>& AudioPlayer::get_play_list()
{
    return _play_list;
};


};
