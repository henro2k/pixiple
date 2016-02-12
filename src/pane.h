#pragma once

#include "shared/com.h"

#include <limits>
#include <memory>
#include <vector>

#include <CommCtrl.h>
#include <d2d1.h>
#include <dwrite.h>
#include <ShObjIdl.h>

class Edge;
class Image;
class Pane;
class Window;

class Pane {
public:
	Edge* edge_left;
	Edge* edge_top;
	Edge* edge_right;
	Edge* edge_bottom;

	Pane() = delete;
	Pane(
		Window* const window,
		Edge* const edge_left,
		Edge* const edge_top,
		Edge* const edge_right,
		Edge* const edge_bottom,
		const D2D1_RECT_F margin,
		const bool fixed_width,
		const bool fixed_height,
		const D2D1_COLOR_F colour);
	Pane(const Pane&) = delete;
	Pane(Pane&& other) = default;

	~Pane();

	Pane& operator=(const Pane&) = delete;
	Pane& operator=(Pane&&) = delete;

	bool has_width() const;
	bool has_height() const;

	float get_width() const;
	float get_height() const;

	D2D1_RECT_F container() const;
	D2D1_RECT_F content() const;
	bool is_inside(const D2D1_POINT_2F& position) const;

	HCURSOR get_cursor() const;
	void set_cursor(LPCTSTR cursor_name);

	void update();
	void draw(ComPtr<ID2D1HwndRenderTarget> render_target) const;

	void set_text(const std::wstring& text, const std::vector<std::pair<std::size_t, std::size_t>>& bold_ranges = std::vector<std::pair<std::size_t, std::size_t>>());

	void set_progressbar_progress(const float progress);

	void add_button(const int button_id, const std::wstring& label);
	void add_combobox(const int button_id, const std::vector<std::wstring>& items);

	std::shared_ptr<Image> get_image() const;
	void set_image(const std::shared_ptr<Image> image);
	float get_image_scale() const;
	void set_image_scale(const float scale);
	void image_zoom_transform(const float scale, const D2D1_POINT_2F& zoom_point_ss, const D2D1_POINT_2F& dpi_scale);
	void set_image_centre_from_other_pane(const Pane& pane_other, const D2D1_POINT_2F& dpi_scale);
	void translate_image_centre(const D2D1_POINT_2F& translation_isn, const D2D1_POINT_2F& dpi_scale);

private:
	static const int progressbar_timer_id = 1;
	static const int progressbar_timer_ms = 200;

	ComPtr<IDWriteFactory> dw_factory;
	ComPtr<IDWriteTextFormat> text_format;

	HFONT button_font = nullptr;

	Window* window;

	float width;
	float height;
	bool fixed_width;
	bool fixed_height;

	D2D1_RECT_F margin;
	D2D1_COLOR_F colour;
	HCURSOR cursor;

	// text
	std::wstring text;
	std::vector<std::pair<std::size_t, std::size_t>> text_bold_ranges;
	ComPtr<IDWriteTextLayout> text_layout;
	HWND text_tooltip_window;
	std::wstring text_tooltip;

	// button
	std::vector<HWND> buttons;
	D2D1_SIZE_F button_size;
	float button_stride;

	// checkbox
	HWND checkbox;

	// progressbar
	HWND progressbar;
	enum class ProgressbarMode {unknown, indeterminate, normal} progressbar_mode;
	ComPtr<ITaskbarList3> progressbar_taskbar_list;

	// image
	std::shared_ptr<Image> image;
	D2D1_POINT_2F image_centre;
	float image_scale;
};
