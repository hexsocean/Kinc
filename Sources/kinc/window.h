#pragma once

#include <kinc/global.h>

#include <stdbool.h>

/*! \file window.h
    \brief Provides functionality for creating and handling windows.
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kinc_framebuffer_options {
	int frequency;
	bool vertical_sync;
	int color_bits;
	int depth_bits;
	int stencil_bits;
	int samples_per_pixel;
} kinc_framebuffer_options_t;

typedef enum {
	KINC_WINDOW_MODE_WINDOW,
	KINC_WINDOW_MODE_FULLSCREEN,
	KINC_WINDOW_MODE_EXCLUSIVE_FULLSCREEN // Only relevant for Windows
} kinc_window_mode_t;

#define KINC_WINDOW_FEATURE_RESIZEABLE 1
#define KINC_WINDOW_FEATURE_MINIMIZABLE 2
#define KINC_WINDOW_FEATURE_MAXIMIZABLE 4
#define KINC_WINDOW_FEATURE_BORDERLESS 8
#define KINC_WINDOW_FEATURE_ON_TOP 16

typedef struct kinc_window_options {
	const char *title;

	int x;
	int y;
	int width;
	int height;
	int display_index;

	bool visible;
	int window_features;
	kinc_window_mode_t mode;
} kinc_window_options_t;

/// <summary>
/// Creates a window.
/// </summary>
/// <returns>The id of the created window</returns>
KINC_FUNC int kinc_window_create(kinc_window_options_t *win, kinc_framebuffer_options_t *frame);

/// <summary>
/// Closes and destroys a window.
/// </summary>
KINC_FUNC void kinc_window_destroy(int window);

KINC_FUNC void kinc_window_options_set_defaults(kinc_window_options_t *win);

KINC_FUNC void kinc_framebuffer_options_set_defaults(kinc_framebuffer_options_t *frame);

/// <summary>
/// Counts all windows the program created, including the initial window created by kinc_init.
/// </summary>
/// <returns>The number of windows</returns>
KINC_FUNC int kinc_count_windows(void);

/// <summary>
/// Resizes a window.
/// </summary>
KINC_FUNC void kinc_window_resize(int window, int width, int height);

/// <summary>
/// Moves a window.
/// </summary>
KINC_FUNC void kinc_window_move(int window, int x, int y);

/// <summary>
/// Switches between window and fullscreen-modes.
/// </summary>
KINC_FUNC void kinc_window_change_mode(int window, kinc_window_mode_t mode);

/// <summary>
/// Applies an ored combination of KINC_WINDOW_FEATURE values.
/// </summary>
KINC_FUNC void kinc_window_change_features(int window, int features);

/// <summary>
/// Changes the framebuffer-options of a window.
/// </summary>
KINC_FUNC void kinc_window_change_framebuffer(int window, kinc_framebuffer_options_t *frame);

/// <summary>
/// Returns the x position of a window.
/// </summary>
/// <returns>The x-position</returns>
KINC_FUNC int kinc_window_x(int window);

/// <summary>
/// Returns the y position of a window.
/// </summary>
/// <returns>The y-position</returns>
KINC_FUNC int kinc_window_y(int window);

/// <summary>
/// Returns the width of a window.
/// </summary>
/// <returns>The window-width</returns>
KINC_FUNC int kinc_window_width(int window);

/// <summary>
/// Returns the height of a window.
/// </summary>
/// <returns>The window-height</returns>
KINC_FUNC int kinc_window_height(int window);

/// <summary>
/// Returns the id of the display the window currently is on.
/// </summary>
/// <returns>The display-id</returns>
KINC_FUNC int kinc_window_display(int window);

/// <summary>
/// Returns the current window-mode.
/// </summary>
/// <returns>The window-mode</returns>
KINC_FUNC kinc_window_mode_t kinc_window_get_mode(int window);

/// <summary>
/// Makes the window visible.
/// </summary>
KINC_FUNC void kinc_window_show(int window);

/// <summary>
/// Hides a window.
/// </summary>
KINC_FUNC void kinc_window_hide(int window);

/// <summary>
/// Sets the title of a window.
/// </summary>
KINC_FUNC void kinc_window_set_title(int window, const char *title);

/// <summary>
/// Sets a resize callback that's called whenever the window is resized.
/// </summary>
KINC_FUNC void kinc_window_set_resize_callback(int window, void (*callback)(int x, int y, void *data), void *data);

/// <summary>
/// Sets a ppi callback that's called whenever the window moves to a display that uses a different ppi-setting.
/// </summary>
KINC_FUNC void kinc_window_set_ppi_changed_callback(int window, void (*callback)(int ppi, void *data), void *data);

/// <summary>
/// Sets a close callback that's called when the window is about to close.
/// Returning false from the callback tries stops the window from closing.
/// </summary>
KINC_FUNC void kinc_window_set_close_callback(int window, bool (*callback)(void *data), void *data);

/// <summary>
/// Returns Whether the window is vsynced or not.
/// </summary>
/// <returns>Whether the window is vsynced or not</returns>
KINC_FUNC bool kinc_window_vsynced(int window);

void kinc_internal_init_window_options(kinc_window_options_t *win);
void kinc_internal_init_framebuffer_options(kinc_framebuffer_options_t *frame);
void kinc_internal_call_resize_callback(int window, int width, int height);
void kinc_internal_call_ppi_changed_callback(int window, int ppi);
bool kinc_internal_call_close_callback(int window);

#ifdef __cplusplus
}
#endif
