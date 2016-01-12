#include <stdlib.h>
#include <jni.h>

#include <android_native_app_glue.h>

#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h> // requires ndk r5 or newer

#include "glwrap/logger.h"
#include "renderer.h"

#define LOG_TAG "EglSample"

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    return 0;
}

/**
 * Process the next main command.
 */
Renderer *renderer;
ANativeWindow* window;
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    switch (cmd)
    {
    case APP_CMD_START:
        renderer = new Renderer();
        break;
    case APP_CMD_RESUME:
        if (renderer)
        {
            renderer->start();
        }
        break;
    case APP_CMD_PAUSE:
        if (renderer)
        {
            renderer->stop();
        }
        break;
    case APP_CMD_STOP:
        if (renderer)
        {
            renderer->stop();
        }
        delete renderer;
        break;
    case APP_CMD_SAVE_STATE:
        // The system has asked us to save our current state.
        break;
    case APP_CMD_INIT_WINDOW:
        // The window is being shown.
        if (0 != renderer)
        {
            LOG_INFO("Init Window %x", (unsigned int ) (app->window));
            renderer->setWindow(app->window);
            window = app->window;
        }
        break;
    case APP_CMD_TERM_WINDOW:
        // The window is being hidden or closed.
        break;
    case APP_CMD_GAINED_FOCUS:
        // Our app gains focus.
        break;
    case APP_CMD_LOST_FOCUS:
        // Our app looses focus.
        break;
    case APP_CMD_WINDOW_REDRAW_NEEDED:

//        ANativeWindow_Buffer buffer;
//        ANativeWindow_lock(window, &buffer, NULL);
//        ANativeWindow_unlockAndPost(window);
        break;

    }
}

void android_main(struct android_app* state)
{
    // Make sure glue isn't stripped.
    app_dummy();
    state->userData = NULL;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;

    // loop waiting for stuff to do.
    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // We will block forever waiting for events.
        while ((ident = ALooper_pollAll(-1, NULL, &events, (void**) &source)) >= 0)
        {

            // Process this event.
            if (source != NULL)
            {
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0)
            {
                LOG_INFO("test_native exited main");

                return;
            }
        }

    }
}
