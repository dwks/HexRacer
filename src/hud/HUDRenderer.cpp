#include "HUDRenderer.h"
#include "opengl/OpenGL.h"

namespace Project {
namespace HUD {

	HUDRenderer::HUDRenderer() {

		alphaMaskShader = new Shader::ShaderProgram((const GLchar*)"shaders/alphaMask.frag", (const GLchar*)"shaders/alphaMask.vert");
		alphaMaskLoc = alphaMaskShader->getUniLoc("alphaMaskTexture");
		renderTextureLoc = alphaMaskShader->getUniLoc("renderTexture");
		usingTextureLoc = alphaMaskShader->getUniLoc("usingTexture");

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
            default:
				x_min = viewWidth/2-(width/2)+h_padding;
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
            default:
				y_min = viewHeight/2-(height/2)+v_padding;
				break;
		}

		glViewport(x_min, y_min, width, height);

	}

	void HUDRenderer::resetViewport() {
		glViewport(0, 0, viewWidth, viewHeight);
	}

	void HUDRenderer::enableAlphaMask(GLuint alpha_mask_texture) {

		alphaMaskShader->turnShaderOn();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, alpha_mask_texture);
		glUniform1i(alphaMaskLoc, 1);
		glUniform1i(usingTextureLoc, 0);
	}

	void HUDRenderer::disableAlphaMask() {

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
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
		glUseProgram(0);

	}

	void HUDRenderer::renderingStateReset() {

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glUseProgram(0);

	}

}  // namespace HUD
}  // namespace Project
