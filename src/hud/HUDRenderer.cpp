#include "HUDRenderer.h"
#include "opengl/OpenGL.h"

namespace Project {
namespace HUD {

	HUDRenderer::HUDRenderer() {

		alphaMaskShader = new Render::Shader("shaders/alphaMask.frag", "shaders/alphaMask.vert");
		alphaMaskLoc = alphaMaskShader->getUniLoc("alphaMaskTexture");
		renderTextureLoc = alphaMaskShader->getUniLoc("renderTexture");
		usingTextureLoc = alphaMaskShader->getUniLoc("usingTexture");
		viewSizeLoc = alphaMaskShader->getUniLoc("viewSize");

	}

	HUDRenderer::~HUDRenderer() {
		delete(alphaMaskShader);
	}

	void HUDRenderer::setViewSize(int width, int height) {
		viewWidth = width;
		viewHeight = height;
	}

	void HUDRenderer::setupViewport( HUDAlign h_align, HUDAlign v_align, int width, int height, int h_padding, int v_padding) {

		int x_min;
		int y_min;

		switch (h_align) {
			case ALIGN_MIN:
				x_min = h_padding;
				break;
			case ALIGN_MAX:
				x_min = viewWidth-width-h_padding;
				break;
			case ALIGN_MID:
				x_min = viewWidth-(width/2);
				break;
			default:
				break;
		}

		switch (v_align) {
			case ALIGN_MIN:
				y_min = v_padding;
				break;
			case ALIGN_MAX:
				y_min = viewHeight-height-v_padding;
				break;
			case ALIGN_MID:
				y_min = viewHeight-(height/2);
				break;
			default:
				break;
		}

		glViewport(x_min, y_min, width, height);

	}

	void HUDRenderer::resetViewport() {
		glViewport(0, 0, viewWidth, viewHeight);
	}

	void HUDRenderer::enableAlphaMask(GLuint alpha_mask_texture, float projection_width, float projection_height) {

		alphaMaskShader->turnShaderOn();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, alpha_mask_texture);
		glUniform1i(alphaMaskLoc, 1);

		GLfloat view_size [2] = {projection_width, projection_height};
		glUniform1fv(viewSizeLoc, 2, view_size);

		glUniform1i(usingTextureLoc, 0);
	}

	void HUDRenderer::disableAlphaMask() {

		glActiveTexture(GL_TEXTURE0);
		alphaMaskShader->turnShaderOff();

	}

	void HUDRenderer::bindTexture(GLuint texture_2D) {
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_2D);

		glUniform1i(usingTextureLoc, 1);
		glUniform1i(renderTextureLoc, 0);

	}

	void HUDRenderer::clearTexture() {
		glUniform1i(usingTextureLoc, 0);
	}

	void HUDRenderer::renderingStateSetup() {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);

	}

	void HUDRenderer::renderingStateReset() {

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

	}

}  // namespace HUD
}  // namespace Project
