#ifndef PROJECT_HUD__HUDRENDERER_H
#define PROJECT_HUD__HUDRENDERER_H

#include "render/Shader.h"

namespace Project {
namespace HUD {

class HUDRenderer {
private:

	Render::Shader* alphaMaskShader;
	int alphaMaskLoc;
	int renderTextureLoc;
	int usingTextureLoc;
	int viewSizeLoc;
	
	int viewWidth;
	int viewHeight;

public:

	enum HUDAlign { ALIGN_MIN, ALIGN_MAX, ALIGN_MID };

	HUDRenderer();
	~HUDRenderer();

	void setViewSize(int width, int height);
	void setupViewport( HUDAlign h_align, HUDAlign v_align, int width, int height, int h_padding, int v_padding);
	void resetViewport();

	void enableAlphaMask(GLuint alpha_mask_texture, float projection_width, float projection_height);
	void disableAlphaMask();
	void bindTexture(GLuint texture_2D);
	void clearTexture();

	void renderingStateSetup();
	void renderingStateReset();
};

}  // namespace HUD
}  // namespace Project

#endif
