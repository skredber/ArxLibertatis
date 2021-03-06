/*
 * Copyright 2011-2016 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARX_GRAPHICS_OPENGL_GLTEXTURE2D_H
#define ARX_GRAPHICS_OPENGL_GLTEXTURE2D_H

#include <boost/intrusive/list_hook.hpp>
#include <GL/glew.h>

#include "graphics/texture/Texture.h"
#include "graphics/texture/TextureStage.h"

class OpenGLRenderer;
class GLTextureStage;

typedef boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > GLTextureListHook;

class GLTexture2D : public Texture2D, public GLTextureListHook {
	
public:
	
	explicit GLTexture2D(OpenGLRenderer * renderer);
	~GLTexture2D();
	
	bool Create();
	void Upload();
	void Destroy();
	
	void apply(GLTextureStage * stage);
	
	void updateMaxAnisotropy();
	
private:
	
	OpenGLRenderer * renderer;
	
	GLuint tex;
	
	TextureStage::WrapMode wrapMode;
	TextureStage::FilterMode minFilter;
	TextureStage::FilterMode magFilter;
	TextureStage::FilterMode mipFilter;
	
	bool isNPOT;
	
	friend class GLTextureStage;
};

#endif // ARX_GRAPHICS_OPENGL_GLTEXTURE2D_H
