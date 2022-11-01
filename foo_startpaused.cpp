#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7

#include <foobar2000/SDK/foobar2000.h>

static constexpr const char* component_name = "Start paused";

DECLARE_COMPONENT_VERSION(
	component_name,
	"1.0",
	"grimes\n\n"
	"Build: " __TIME__ ", " __DATE__
);

VALIDATE_COMPONENT_FILENAME("foo_startpaused.dll");

// {FC0DC88B-7CAF-49F6-BBAF-8E27E7C2E83E}
static const GUID g_guid_main_startpaused =
{ 0xfc0dc88b, 0x7caf, 0x49f6, { 0xbb, 0xaf, 0x8e, 0x27, 0xe7, 0xc2, 0xe8, 0x3e } };

class mainmenu_startpaused : public mainmenu_commands {
	virtual t_uint32 get_command_count() { return 1; }
	virtual GUID get_command(t_uint32 p_index) {
		if (p_index == 0)
			return g_guid_main_startpaused;
		return pfc::guid_null;
	}
	virtual void get_name(t_uint32 p_index, pfc::string_base& p_out) {
		if (p_index == 0)
			p_out = "Start paused";
	}
	virtual bool get_description(t_uint32 p_index, pfc::string_base& p_out) {
		if (p_index == 0)
			p_out = "Start playback paused";
		return true;
	}
	virtual GUID get_parent() { return mainmenu_groups::playback; }

	virtual void execute(t_uint32 p_index, service_ptr_t<service_base>) {
		if (p_index == 0)
		{
			static_api_ptr_t<playback_control>()->play_start(playback_control::t_track_command::track_command_play, true);
		}
	}
	virtual bool get_display(t_uint32 p_index, pfc::string_base& p_text, t_uint32&)
	{
		if (p_index == 0)
		{
			get_name(p_index, p_text);
		}
		return true;
	}
	virtual t_uint32 get_sort_priority()
	{
		return 0x80000000;
	}
};
static mainmenu_commands_factory_t< mainmenu_startpaused > g_mainmenu_startpaused;