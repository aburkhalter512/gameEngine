#pragma once

#include <stdlib.h>

/*
Gets the OpenGL id associated with a given texture. This function loads the texture from disk
if was not previously retrieved. File name is relative to the cwd of the executable

Arguments
    const char* fileName: The file name of the texture

Returns
    Returns the OpenGL id for the texture;
*/
uint32_t get_texture(const char* fileName);
