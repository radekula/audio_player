#include "interface.hpp"
#include "app_state.hpp"
#include "audio_player.hpp"


extern "C" {
std::unique_ptr<iface::GuiInterface> CreateInterface();
};


class TermGuiInterface : public iface::GuiInterface
{
private:
	std::shared_ptr<app::AppState> _app_state;
	std::shared_ptr<player::AudioPlayer> _audio_player;

public:
	~TermGuiInterface();
	
public:
    std::string interface_info();
    
public:
    bool init(int argc, char *argv[]);
    void register_other_interfaces(const std::vector<iface::GuiInterfaceInfo> &interfaces);
	void register_app_state(std::shared_ptr<app::AppState> state);
	void register_audio_player(std::shared_ptr<player::AudioPlayer> audio_player);

    void run();
};
