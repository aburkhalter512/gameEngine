#include "util/loadShaders.h"

#include <stdio.h>
#include <stdlib.h>

// Reads a shader from a file and stores it in dynamically allocated memory
static const GLchar* readShader(const char* filename)
{
    FILE* infile = fopen(filename, "rb");

    if (!infile)
    {
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell(infile);
    fseek( infile, 0, SEEK_SET );

    GLchar* source = malloc(sizeof(GLchar) * (len + 1));

    fread(source, 1, len, infile);
    fclose(infile);

    source[len] = 0;

    return (const GLchar*) source;
}

GLuint loadShaders(ShaderInfo* shaders)
{
    if (shaders == NULL)
    {
        return 0;
    }

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while (entry->type != GL_NONE)
    {
        GLuint shader = glCreateShader(entry->type);

        entry->shader = shader;

        // Load the shader into a string
        const GLchar* source = readShader(entry->filename);
        if (source == NULL)
        {
            for (entry = shaders; entry->type != GL_NONE; ++entry)
            {
                glDeleteShader(entry->shader);
                entry->shader = 0;
            }

            printf("Could not read shader %s\n", entry->filename);

            return 0;
        }

        // Give the string to OpenGL
        glShaderSource(shader, 1, &source, NULL);
        free((void*) source);

        // Compile the shader
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            printf("Could not compile shader %s\n", entry->filename);

            GLint logInfoLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logInfoLength);

            GLchar *log = malloc(sizeof(GLchar) * logInfoLength + 1);
            glGetShaderInfoLog(shader, logInfoLength, &logInfoLength, log);

            printf("%s\n", log);

            free(log);
            return 0;
        }

        // Attach the current shader to the program
        glAttachShader(program, shader);

        ++entry;
    }

    // Compile all attached shaders
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        for (entry = shaders; entry->type != GL_NONE; ++entry)
        {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }

        printf("Could not link shaders\n");

        GLint logInfoLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logInfoLength);

        GLchar *log = malloc(sizeof(GLchar) * logInfoLength + 1);
        glGetShaderInfoLog(program, logInfoLength, &logInfoLength, log);

        printf("%s\n", log);

        return 0;
    }

    return program;
}

void deleteShaders(ShaderInfo* shaders)
{
    if (shaders == NULL)
    {
        return;
    }

    for (ShaderInfo* entry = shaders; entry->type != GL_NONE; entry++)
    {
        glDeleteShader(entry->shader);
        entry->shader = 0;
    }
}
