#ifndef PROJECT_HUD__LAP_PROGRESS_BAR_H
#define PROJECT_HUD__LAP_PROGRESS_BAR_H

namespace Project {
namespace HUD {

class LapProgressBar {
private:
	float progress;
public:
	void setProgress(float _progress) { progress = _progress; }
	void render();
};

}  // namespace HUD
}  // namespace Project

#endif
