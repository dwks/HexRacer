#include "RaceClock.h"
#include "render/FontManager.h"
#include "misc/StreamAsString.h"
#include "log/Logger.h"

namespace Project {
namespace HUD {

	RaceClock::RaceClock()
		: entry(Render::TextureObjectChain::HORIZONTAL,
		Render::TextureObjectChain::MIN,
		Render::TextureObjectChain::MIN) {
		width = 100;
		height = 100;
		entry.append(&timeTexture);

		totalTime = 0;
		timeOffset = 0;
	}

	void RaceClock::setSeconds(unsigned int _seconds) {
		std::ostringstream str;
		str << (_seconds/60) << ':';
		unsigned int secs = (_seconds % 60);
		if (secs < 10)
			str << '0';
		str << secs;
		timeTexture.setString(str.str(), Render::FontManager::getInstance()->getCurrentFont());
	}

	void RaceClock::render() {

		setSeconds((totalTime-timeOffset)/1000);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		entry.update();
		entry.render(0.0, 0.0, height);

	}

	void RaceClock::addTimeOffset(unsigned long offset) {
		timeOffset += offset;
		//LOG(GLOBAL, "paused offset: " << offset);
	}

}  // namespace HUD
}  // namespace Project
