#include <OpenGL/gl3.h>
#include <stdio.h>
#include <unistd.h>

#include "SDL2/SDL.h"

#include "engine/collision.h"
#include "engine/gameEnvironment.h"
#include "engine/gameObject.h"

#include "game/gameObjectTypes.h"

#include "game/ball.h"
#include "game/border.h"
#include "game/paddle.h"

SDL_Window *win;

void onUpdate(gameEnvironment* env, gameObject* g)
{
    switch (getType_gameObject(g))
    {
        case GameObject_Paddle:
            update_paddle(g);
            break;
        case GameObject_Ball:
            update_ball(g);
            break;
    }
}

void onCollision(gameEnvironment* env, gameObject* g1, gameObject* g2, collision* c)
{
    printf("onCollision: g1 type: %d, g2 type: %d\n", getType_gameObject(g1), getType_gameObject(g2));
    switch (getType_gameObject(g1))
    {
        case GameObject_Ball:
            onCollision_ball(g1, g2, c);
            return;
        case GameObject_Paddle:
            onCollision_paddle(g1, g2, c);
            return;
        default:
            printf("onCollision not handled: g1 type: %d, g2 type: %d\n", getType_gameObject(g1), getType_gameObject(g2));
            break;
    }
}

void onRenderStart(gameEnvironment* env) {}

void onRenderEnd(gameEnvironment* env)
{
    SDL_GL_SwapWindow(win);
}

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    win = SDL_CreateWindow("Basic Pong Game", 100, 100, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        printf("%s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(win);
    if (glcontext == NULL)
    {
        printf("%s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    gameEvents ge = { onUpdate, onCollision, onRenderStart, onRenderEnd };
    gameSettings gs = { 768.0f / 1024.0f };

    gameEnvironment* env = create_gameEnvironment(ge, gs);

    gameObject* paddle = create_paddle();
    addGameObject_gameEnvironment(env, paddle);

    border b = create_border();
    addGameObject_gameEnvironment(env, b.left);
    addGameObject_gameEnvironment(env, b.right);
    addGameObject_gameEnvironment(env, b.top);

    gameObject* ball = create_ball();
    addGameObject_gameEnvironment(env, ball);

    bool isRunning = true;
    // size_t frameCount = 0;
    while(isRunning)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }

        run_gameEnvironment(env);
    }

    free_gameObject(ball);
    free_gameObject(b.left);
    free_gameObject(b.right);
    free_gameObject(b.top);
    free_gameObject(paddle);
    free_gameEnvironment(env);

    SDL_GL_DeleteContext(glcontext);
    SDL_Quit();

    return 0;
}
