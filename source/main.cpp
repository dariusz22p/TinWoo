#include <thread>
#include "switch.h"
#include "util/error.hpp"
#include "ui/MainApplication.hpp"
#include "util/util.hpp"
#include "util/config.hpp"

using namespace pu::ui::render;
int main(int argc, char* argv[])
{
	inst::util::initApp();
	try {
		//const auto default_font_path = ("romfs:/fonts/Roboto-Regular.ttf");
		auto renderer_opts = pu::ui::render::RendererInitOptions(SDL_INIT_EVERYTHING, pu::ui::render::RendererHardwareFlags);
		renderer_opts.UseImage(pu::ui::render::IMGAllFlags);
    renderer_opts.UseAudio(pu::ui::render::MixerAllFlags);
    //renderer_opts.UseTTF(default_font_path);
    renderer_opts.UseTTF();
		//renderer_opts.SetExtraDefaultFontSize(35);
		renderer_opts.UseRomfs();
		auto renderer = pu::ui::render::Renderer::New(renderer_opts);
		
		auto main = inst::ui::MainApplication::New(renderer);
		std::thread updateThread;
		if (inst::config::autoUpdate && inst::util::getIPAddress() != "1.0.0.127") updateThread = std::thread(inst::util::checkForAppUpdate);
		main->Prepare();
		main->ShowWithFadeIn();
		updateThread.join();
	} catch (std::exception& e) {
			LOG_DEBUG("An error occurred:\n%s", e.what());
		}
		inst::util::deinitApp();
		return 0;
	}
