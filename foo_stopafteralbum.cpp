#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7

#include <helpers/foobar2000+atl.h>
#include <SDK/titleformat.cpp>

static constexpr const char* component_name = "Stop after album";

DECLARE_COMPONENT_VERSION(
	component_name,
	"1.2",
	"grimes\n\n"
	"Build: " __TIME__ ", " __DATE__
);

VALIDATE_COMPONENT_FILENAME("foo_stopafteralbum.dll");

pfc::string8 tracknumber;
int tracknumber2;
pfc::string8 totaltracks;
int totaltracks2;

static constexpr GUID g_guid_main_stopafteralbum_toggle = { 0x92d2cf2e, 0xad1e, 0x4121, { 0x87, 0x52, 0xc3, 0xe1, 0x4d, 0xdc, 0x80, 0x74 } };

// {DD8A6BF4-4A7E-4936-A159-E14C0416D295}
static constexpr GUID guid_cfg_menu_stopalbum_enabled = { 0xdd8a6bf4, 0x4a7e, 0x4936, { 0xa1, 0x59, 0xe1, 0x4c, 0x4, 0x16, 0xd2, 0x95 } };
cfg_bool cfg_menu_stopalbum_enabled(guid_cfg_menu_stopalbum_enabled, false);

class mainmenu_mycommands : public mainmenu_commands {
	virtual t_uint32 get_command_count() { return 1; }
	virtual GUID get_command(t_uint32 p_index) {
		if (p_index == 0)
			return g_guid_main_stopafteralbum_toggle;
		return pfc::guid_null;
	}
	virtual void get_name(t_uint32 p_index, pfc::string_base& p_out) {
		if (p_index == 0)
			p_out = "Stop after album";
	}
	virtual bool get_description(t_uint32 p_index, pfc::string_base& p_out) {
		if (p_index == 0)
			p_out = "Stops playback after album";
		return true;
	}
	virtual GUID get_parent() { return mainmenu_groups::playback; }

	virtual void execute(t_uint32 p_index, service_ptr_t<service_base>) {
		if (p_index == 0)
		{
			cfg_menu_stopalbum_enabled = !cfg_menu_stopalbum_enabled;
			if (cfg_menu_stopalbum_enabled)
			{
				if ((totaltracks2 == tracknumber2) && (pfc::string_is_numeric(tracknumber) || pfc::string_is_numeric(totaltracks)) && (static_api_ptr_t<playback_control>()->is_paused() || static_api_ptr_t<playback_control>()->is_playing()))
				{
					static_api_ptr_t<playback_control>()->set_stop_after_current(true);
					console::info("Stop after album");
				}
			}
			else
			{
				static_api_ptr_t<playback_control>()->set_stop_after_current(false);
			}
		}
	}
	virtual bool get_display(t_uint32 p_index, pfc::string_base& p_text, t_uint32& p_flags)
	{
		if (p_index == 0)
		{
			get_name(p_index, p_text);
			if (cfg_menu_stopalbum_enabled)
			{
				p_flags |= flag_checked;
			}
			else {
				p_flags = 0;
			}
		}
		return true;
	}
	virtual t_uint32 get_sort_priority()
	{
		return sort_priority_dontcare;
	}
	bool is_checked(t_uint32 p_index)
	{
		if (p_index == 0)
			return cfg_menu_stopalbum_enabled;
	}
};
static mainmenu_commands_factory_t< mainmenu_mycommands > g_mainmenu_mycommands;

class play_callback_stopafteralbum : public play_callback_static
{

public:
	unsigned get_flags() { return flag_on_playback_new_track; }
	virtual void FB2KAPI on_playback_new_track(metadb_handle_ptr p_track)
	{
		titleformat_object::ptr titleformat;
		titleformat_compiler::get()->compile_safe_ex(titleformat, "%tracknumber%", "<ERROR>");
		p_track->format_title(nullptr, tracknumber, titleformat, nullptr);
		titleformat_object::ptr titleformat2;
		titleformat_compiler::get()->compile_safe_ex(titleformat2, "%totaltracks%", "<ERROR>");
		p_track->format_title(nullptr, totaltracks, titleformat2, nullptr);
		tracknumber2 = atoi(tracknumber);
		totaltracks2 = atoi(totaltracks);
		if (cfg_menu_stopalbum_enabled) {
			if (tracknumber2 == totaltracks2 && (pfc::string_is_numeric(tracknumber) || pfc::string_is_numeric(totaltracks)))
			{
				static_api_ptr_t<playback_control>()->set_stop_after_current(true);
				console::info("Stop after album");
			}
			else
			{
				static_api_ptr_t<playback_control>()->set_stop_after_current(false);
			}
		}
	}
	virtual void on_playback_stop(play_control::t_stop_reason p_reason) {}
	virtual void on_playback_pause(bool p_state) {}
	virtual void on_playback_starting(play_control::t_track_command p_command, bool p_paused) {}
	virtual void on_playback_seek(double p_time) {}
	virtual void on_playback_edited(metadb_handle_ptr p_track) {}
	virtual void on_playback_dynamic_info(const file_info& info) {}
	virtual void on_playback_dynamic_info_track(const file_info& info) {}
	virtual void on_playback_time(double p_time) {}
	virtual void on_volume_change(float p_new_val) {}
};
static play_callback_static_factory_t<play_callback_stopafteralbum> g_play_callback_stopafteralbum;