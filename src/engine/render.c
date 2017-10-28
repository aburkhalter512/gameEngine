#include "engine/render.h"

#include "engine/math/transform.h"
#include "engine/math/vec.h"
#include "engine/texture.h"
#include "util/loadShaders.h"

#include <OpenGL/gl3.h>
#include <stdio.h>

static const int INITIAL_INDEX = 0;
static const int VERTEX_SIZE = 4;
static const int VERTEX_COUNT = 6;
static const int TIGHTLY_PACKED_ARRAY = 0;
static const void* NO_INITIAL_OFFSET = NULL;
static const int RESET = 0;
static const int INVALID_UNIFORM = -1;

static float quadVertices[VERTEX_COUNT][VERTEX_SIZE] = {
    // Pos              // Tex
    { -0.5f, -0.5f,     0.0f, 0.0f },
    {  0.5f, -0.5f,     1.0f, 0.0f },
    {  0.5f,  0.5f,     1.0f,  1.0f },

    { -0.5f, -0.5f,     0.0f, 0.0f },
    {  0.5f,  0.5f,     1.0f, 1.0f },
    { -0.5f,  0.5f,     0.0f, 1.0f },
};

static uint32_t quadVertexArray = 0;
static uint32_t quadVertexBuffer = 0;

typedef struct _render
{
    transform* t;

    uint32_t shaderId;
    int32_t mvpUniform;
    int32_t sizeUniform;
    int32_t imageUniform;
    int32_t colorUniform;

    uint32_t textureId;

    MATRIX_TYPE(3, 3) size; // size.rows[0].x/size.rows[1].y are the x and y components of the render size
} render;

render* create_render(transform* t, renderInfo rI)
{
    if (!t || GET_X(rI.size) <= 0.0f || GET_Y(rI.size) <= 0.0f || !rI.vertexShader || !rI.fragmentShader || rI.textureId == 0)
    {
        printf("create_render(): could not create render\n");
        return NULL;
    }

    // Every render will use the same vao/vbo, but use a different model and texture
    // to display them individually
    if (quadVertexArray == 0)
    {
        // Create the OpenGL vao/vbos
        glGenVertexArrays(1, &quadVertexArray);
        glGenBuffers(1, &quadVertexBuffer);

        // Link quadVertices to quadVertexBuffer
        glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // Link quadVertexArray with quadVertexBuffer
        glBindVertexArray(quadVertexArray);

        // Define how the vertex data is stored
        glEnableVertexAttribArray(INITIAL_INDEX);
        glVertexAttribPointer(INITIAL_INDEX, VERTEX_SIZE, GL_FLOAT, GL_FALSE, TIGHTLY_PACKED_ARRAY, NO_INITIAL_OFFSET);

        // Reset the buffer and vertex arrays inside OpenGL (does not delete the objects)
        glBindBuffer(GL_ARRAY_BUFFER, RESET);
        glBindVertexArray(RESET);
    }

    render* r = malloc(sizeof(render));
    if (!r)
    {
        return NULL;
    }

    r->size = (MATRIX_TYPE(3, 3)) {{
        {GET_X(rI.size), 0, 0},
        {0, GET_Y(rI.size), 0},
        {0, 0, 1},
    }};

    r->t = t;

    // Load the actual shaders
    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, rI.vertexShader },
        { GL_FRAGMENT_SHADER, rI.fragmentShader },
        { GL_NONE, NULL },
    };
    r->shaderId = loadShaders(shaders);
    if (r->shaderId == 0)
    {
        return false;
    }

    // Get the shader variable locations
    r->mvpUniform   = glGetUniformLocation(r->shaderId, "mvp");
    r->sizeUniform  = glGetUniformLocation(r->shaderId, "size");
    r->imageUniform = glGetUniformLocation(r->shaderId, "image");
    r->colorUniform = glGetUniformLocation(r->shaderId, "color");

    // Make sure all of the uniforms actually exist
    if (r->mvpUniform   == INVALID_UNIFORM ||
        r->sizeUniform  == INVALID_UNIFORM ||
        r->imageUniform == INVALID_UNIFORM ||
        r->colorUniform == INVALID_UNIFORM)
    {
        // TODO: free shaders, do some error handling
    }

    r->textureId = rI.textureId;

    return r;
}

void render_render(render* r, float aspect)
{
    if (!r || r->shaderId == 0)
    {
        return;
    }

    // Make this render's shader active
    glUseProgram(r->shaderId);

    MATRIX_TYPE(3, 3) modelMat = getMatrix_transform(r->t);

    // View TODO: replace with movable camera matrix
    MATRIX_TYPE(3, 3) viewMat = {{
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
    }};

    MATRIX_TYPE(3, 3) projectionMat = {{
        {aspect, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
    }};

    MATRIX_TYPE(3, 3) modelViewMat = MULTIPLY_MATRIX_FN(3, 3, 3)(&viewMat, &modelMat);

    MATRIX_TYPE(3, 3) mvp = MULTIPLY_MATRIX_FN(3, 3, 3)(&projectionMat, &modelViewMat);

    // TODO: replace with user set color
    vec3f color = to_vec3f(1.0f, 1.0f, 1.0f);

    // Set the uniform variables in the shader
    glUniformMatrix3fv(r->mvpUniform,   1, GL_TRUE, (const GLfloat*) mvp.data);
    glUniformMatrix3fv(r->sizeUniform,  1, GL_TRUE, (const GLfloat*) r->size.data);
    glUniform3f(r->colorUniform, GET_X(color), GET_Y(color), GET_Z(color));

    // Set the texture we want in the shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, r->textureId);
    glUniform1i(r->imageUniform, 0);

    // Draw the quad
    glBindVertexArray(quadVertexArray);
    glDrawArrays(GL_TRIANGLES, INITIAL_INDEX, VERTEX_COUNT);
    glBindVertexArray(RESET);
}
