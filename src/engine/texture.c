#include "engine/texture.h"

#include "datastructures/hashtable.h"

#include <string.h>
#include <OpenGL/gl3.h>
#include <png.h>

typedef struct _texture
{
    size_t width;
    size_t height;

    uint8_t* texels; // Row-major

    uint32_t textureId;
} texture;

hashtable* textureTable = NULL;

texture* _loadPng_texture(const char* fileName)
{
    png_image image;
    memset(&image, 0, (sizeof image));

    image.version = PNG_IMAGE_VERSION;

    /* The first argument is the file to read: */
    if (png_image_begin_read_from_file(&image, fileName) == 0)
    {
        printf("_loadPng_texture(): could not create begin to read png image %s\n", fileName);
        return NULL;
    }

    // We want the RGBA format
    image.format = PNG_FORMAT_RGBA;
    png_bytep buffer = malloc(PNG_IMAGE_SIZE(image));
    if (!buffer)
    {
        printf("_loadPng_texture(): could not create png image buffer\n");
        return NULL;
    }

    if (png_image_finish_read(&image, NULL/*background*/, buffer, 0/*row_stride*/, NULL/*colormap*/) == 0)
    {
        printf("_loadPng_texture(): could not finish reading the image\n");
        free(buffer);
        return NULL;
    }

    texture* t = malloc(sizeof(texture));
    *t = (texture) { image.width, image.height, buffer };

    return t;
}

uint32_t get_texture(const char* fileName)
{
    if (!fileName)
    {
        return 0;
    }

    size_t fileNameLen = strlen(fileName);
    if (fileNameLen <= 4)
    {
        printf("get_texture(): fileName is <= 4 characters %s\n", fileName);
        return 0;
    }

    if (!textureTable)
    {
        textureTable = create_hashtable(32, hasher_string, comparator_string);
    }

    // See if the texture has already been loaded
    texture* t = get_hashtable(textureTable, (void*) fileName);
    if (t)
    {
        return t->textureId;
    }

    // Load the texture by extension
    const char* extension = &fileName[fileNameLen - 3];
    printf("extension: %s\n", extension);
    if (strcmp(extension, "png") == 0)
    {
        t = _loadPng_texture(fileName);
    }

    // Were we able to load the texture?
    if (!t)
    {
        printf("get_texture(): could not get texture %s\n", fileName);
        return 0;
    }

    // Create the OpenGL texture
    glGenTextures(1, &t->textureId);

    glBindTexture(GL_TEXTURE_2D, t->textureId);

    // Linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Don't tile the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Link the texels to the OpenGL texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->texels);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Save the texture in the global textrue table
    set_hashtable(textureTable, (void*) fileName, t);

    return t->textureId;
}
