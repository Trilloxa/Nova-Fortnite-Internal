#pragma once
#include "../stdafx.h"

namespace GUI
{
	namespace Input
	{
		bool mouseDown[5];
		bool mouseDownAlready[256];

		bool keysDown[256];
		bool keysDownAlready[256];

		inline bool IsAnyMouseDown()
		{
			if (mouseDown[0]) return true;
			if (mouseDown[1]) return true;
			if (mouseDown[2]) return true;
			if (mouseDown[3]) return true;
			if (mouseDown[4]) return true;

			return false;
		}

		inline bool IsMouseClicked(int button, int element_id, bool repeat)
		{
			if (mouseDown[button])
			{
				if (!mouseDownAlready[element_id])
				{
					mouseDownAlready[element_id] = true;
					return true;
				}
				if (repeat)
					return true;
			}
			else
			{
				mouseDownAlready[element_id] = false;
			}
			return false;
		}
		inline bool IsKeyPressed(int key, bool repeat)
		{
			if (keysDown[key])
			{
				if (!keysDownAlready[key])
				{
					keysDownAlready[key] = true;
					return true;
				}
				if (repeat)
					return true;
			}
			else
			{
				keysDownAlready[key] = false;
			}
			return false;
		}

		inline void Handle()
		{
			if (nt::GetKey(VK_LBUTTON))
				mouseDown[0] = true;
			else
				mouseDown[0] = false;
		}
	}
}

//wchar_t* s2wc(const char* c)
//{
//	const size_t cSize = strlen(c) + 1;
//	wchar_t* wc = new wchar_t[cSize];
//	mbstowcs(wc, c, cSize);
//
//	return wc;
//}

namespace GUI
{
	namespace Colors
	{
		/*FLinearColor Text{ 1.0f, 1.0f, 1.0f, 1.0f };
		FLinearColor Text_Shadow{ 0.0f, 0.0f, 0.0f, 0.0f };
		FLinearColor Text_Outline{ 0.0f, 0.0f, 0.0f, 0.0f };

		FLinearColor Window_Background{ 200.0f, 200.0f, 200.0f, 1.0f };
		FLinearColor Window_Header{ 200.0f, 200.0f, 200.0f, 1.0f };
		FLinearColor Window_Outline{ 0.f, 0.f, 0.f, 1.f };

		FLinearColor Button_Idle{ 255.0f, 255.0f, 255.0f, 1.0f };
		FLinearColor Button_Hovered{ 255.0f, 255.0f, 255.0f, 1.0f };
		FLinearColor Button_Active{ 1.0f, 1.0f, 1.0f, 1.0f };

		FLinearColor Checkbox_Idle{ 186.0f, 186.0f, 186.0f, 1.0f };
		FLinearColor Checkbox_Hovered{ 0.0f, 0.0f, 0.0f, 0.0f };
		FLinearColor Checkbox_Enabled{ 0.0f, 0.0f, 0.0f, 1.0f };

		FLinearColor Combobox_Idle{ 186.0f, 186.0f, 186.0f, 1.0f };
		FLinearColor Combobox_Hovered{ 128.0f, 128.0, 128.0, 0.0f };
		FLinearColor Combobox_Elements{ 128.0, 128.0, 128.0, 1.0f };

		FLinearColor Slider_Idle{ 230.0f, 230.0f, 230.0f, 1.0f };
		FLinearColor Slider_Hovered{ 230.0f, 230.0f, 230.0f, 1.0f };
		FLinearColor Slider_Progress{ 0.0f, 0.0f, 0.0f, 1.0f };
		FLinearColor Slider_Button{ 255.0f, 255.0f, 255.0f, 1.0f };
		FLinearColor Slider_Button_Outline{ 0.0f, 0.0f, 0.0f, 1.0f };*/

		FLinearColor Text{ 255.0f, 0.0f, 0.0f, 1.0f };
		FLinearColor Text_Shadow{ 255.0f, 0.0f, 0.0f, 0.0f };
		FLinearColor Text_Outline{ 255.0f, 0.0f, 0.0f, 0.0f };

		FLinearColor Window_Background{ 78.0f, 78.0f, 78.0f, 1.0f };
		FLinearColor Window_Header{ 78.0f, 78.0f, 78.0f, 1.0f };
		FLinearColor Window_Outline{ 0.0f, 0.0f, 0.0f, 1.0f };
		FLinearColor Window_Line{ 0.0f, 0.0f, 0.0f, 1.0f };

		FLinearColor Button_Idle{ 78.0f, 78.0f, 78.0f, 1.0f };
		FLinearColor Button_Hovered{ 78.0f, 78.0f, 78.0f, 1.0f };
		FLinearColor Button_Active{ 78.0f, 78.0f, 78.0f, 1.0f };

		FLinearColor Checkbox_Idle{ 0.0f, 0.0f, 0.0f, 1.0f };
		FLinearColor Checkbox_Hovered{ 0.0f, 0.0f, 0.0f, 0.0f };
		FLinearColor Checkbox_Enabled{ 200.0f, 200.0f, 200.0f, 1.0f };

		FLinearColor Combobox_Idle{ 78.0f, 78.0f, 78.0f, 1.0f };
		FLinearColor Combobox_Hovered{ 78.0f, 78.0f, 78.0f, 1.0f };
		FLinearColor Combobox_Elements{ 20.0f, 20.0f, 20.0f, 1.0f };

		FLinearColor Slider_Idle{ 255.0f, 255.0f, 255.0f, 1.0f };
		FLinearColor Slider_Hovered{ 255.0f, 255.0f, 255.0f, 1.0f };
		FLinearColor Slider_Progress{ 0.0f, 0.0f, 0.0f, 1.0f };
		FLinearColor Slider_Button{ 255.0f, 255.0f, 255.0f, 1.0f };
		FLinearColor Slider_Button_Outline{ 0.0f, 0.0f, 0.0f, 1.0f };
	}

	namespace PostRenderer
	{
		struct DrawList
		{
			int type = -1;
			FVector2D pos;
			FVector2D size;
			FLinearColor color;
			const char* name;
			bool outline;

			FVector2D from;
			FVector2D to;
			int thickness;
		};
		DrawList drawlist[128];

		inline void drawFilledRect(FVector2D pos, float w, float h, FLinearColor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 1;
					drawlist[i].pos = pos;
					drawlist[i].size = FVector2D{ w, h };
					drawlist[i].color = color;
					return;
				}
			}
		}
		inline void TextLeft(const char* name, FVector2D pos, FLinearColor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 2;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		inline void TextCenter(const char* name, FVector2D pos, FLinearColor color, bool outline)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 3;
					drawlist[i].name = name;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}
		inline void Draw_Line(FVector2D from, FVector2D to, int thickness, FLinearColor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 4;
					drawlist[i].from = from;
					drawlist[i].to = to;
					drawlist[i].thickness = thickness;
					drawlist[i].color = color;
					return;
				}
			}
		}
	}

	void* canvas;

	bool hover_element = false;
	FVector2D menu_pos = FVector2D{ 60, 150 };
	float offset_x = 0.0f;
	float offset_y = 0.0f;

	FVector2D first_element_pos = FVector2D{ 0, 0 };

	FVector2D last_element_pos = FVector2D{ 0, 0 };
	FVector2D last_element_size = FVector2D{ 0, 0 };

	int current_element = -1;
	FVector2D current_element_pos = FVector2D{ 0, 0 };
	FVector2D current_element_size = FVector2D{ 0, 0 };
	int elements_count = 0;

	bool sameLine = false;

	bool pushY = false;
	float pushYvalue = 0.0f;

	void SetupCanvas(void* _canvas)
	{
		canvas = _canvas;
	}

	FVector2D CursorPos()
	{

		POINT p;
		//Engine::GetCursor(&p);
		GetCursorPos(&p);
		return FVector2D{ (float)p.x, (float)p.y };
	}
	bool MouseInZone(FVector2D pos, FVector2D size)
	{
		FVector2D cursor_pos = CursorPos();

		if (cursor_pos.X > pos.X && cursor_pos.Y > pos.Y)
			if (cursor_pos.X < pos.X + size.X && cursor_pos.Y < pos.Y + size.Y)
				return true;

		return false;
	}

	inline void Draw_Cursor(bool toogle)
	{
		if (toogle)
		{
			FVector2D cursorPos = CursorPos();
			Engine::K2_DrawBox(canvas, FVector2D(cursorPos.X, cursorPos.Y), FVector2D(4, 4), 4.0f, FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	inline void SameLine()
	{
		sameLine = true;
	}
	inline void PushNextElementY(float y, bool from_last_element = true)
	{
		pushY = true;
		if (from_last_element)
			pushYvalue = last_element_pos.Y + last_element_size.Y + y;
		else
			pushYvalue = y;
	}
	inline void NextLine(float y)
	{
		offset_y = y;
	}

	inline void AddY(float y)
	{
		offset_y += y;
	}

	inline void NextColumn(float x)
	{
		offset_x += x;
		// PushNextElementY(first_element_pos.Y, false);
	}
	inline void ClearFirstPos()
	{
		first_element_pos = FVector2D{ 0, 0 };
	}

	inline void TextLeft(const char* name, FVector2D pos, FLinearColor color, bool outline)
	{
		int length = strlen(name) + 1;
		Engine::K2_DrawText(canvas, FString{ s2wc(name) }, pos, FVector2D{ 0.97f, 0.97f }, color, false, Colors::Text_Shadow, FVector2D{ pos.X + 1, pos.Y + 1 }, false, true, true, Colors::Text_Outline);
	}
	inline void TextCenter(const char* name, FVector2D pos, FLinearColor color, bool outline)
	{
		int length = strlen(name) + 1;
		Engine::K2_DrawText(canvas, FString{ s2wc(name) }, pos, FVector2D{ 0.97f, 0.97f }, color, false, Colors::Text_Shadow, FVector2D{ pos.X + 1, pos.Y + 1 }, true, true, true, Colors::Text_Outline);
	}

	inline void Draw_Line(FVector2D from, FVector2D to, int thickness, FLinearColor color)
	{
		Engine::K2_DrawLine(canvas, FVector2D{ from.X, from.Y }, FVector2D{ to.X, to.Y }, thickness, color);
	}
	inline void drawFilledRect(FVector2D initial_pos, float w, float h, FLinearColor color)
	{
		for (float i = 0.0f; i < h; i += 1.0f)
			Engine::K2_DrawLine(canvas, FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, 1.0f, color);
	}
	inline void drawRect(FVector2D initial_pos, float t, FVector2D size, FLinearColor color)
	{
		Engine::K2_DrawBox(canvas, { initial_pos.X / 2, initial_pos.Y / 2 }, size, t, color);
	}
	inline void DrawFilledCircle(FVector2D pos, float r, FLinearColor color)
	{
		float smooth = 0.07f;

		float PI = 3.14159265359;
		int size = (int)(2.0f * PI / smooth) + 1;

		float angle = 0.0f;
		int i = 0;

		for (; angle < 2 * PI; angle += smooth, i++)
		{
			Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X + cosf(angle) * r, pos.Y + sinf(angle) * r }, 1.0f, color);
		}
	}

	inline void DrawCircle(FVector2D pos, int radius, int numSides, FLinearColor Color, float thickness = 1.0f)
	{
		float PI = 3.1415927f;

		float Step = PI * 2.0 / numSides;
		int Count = 0;
		FVector2D V[128];
		for (float a = 0; a < PI * 2.0; a += Step)
		{
			float X1 = radius * crt::cos(a) + pos.X;
			float Y1 = radius * crt::sin(a) + pos.Y;
			float X2 = radius * crt::cos(a + Step) + pos.X;
			float Y2 = radius * crt::sin(a + Step) + pos.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			Draw_Line(FVector2D{ V[Count].X, V[Count].Y }, FVector2D{ X2, Y2 }, thickness, Color);
		}
	}

	FVector2D dragPos;
	bool Window(const char* name, FVector2D* pos, FVector2D size, bool isOpen)
	{
		elements_count = 0;

		if (!isOpen)
			return false;
		bool isHovered = MouseInZone(FVector2D{ pos->X, pos->Y }, size);
		if (current_element != -1 && !nt::GetKey(VK_LEFT))
		{
			current_element = -1;
		}

		if (hover_element && nt::GetKey(VK_LEFT))
		{

		}
		else if ((isHovered || dragPos.X != 0) && !hover_element)
		{

			if (Input::IsMouseClicked(0, elements_count, true))
			{
				FVector2D cursorPos = CursorPos();

				cursorPos.X -= size.X;
				cursorPos.Y -= size.Y;

				if (dragPos.X == 0)
				{
					dragPos.X = (cursorPos.X - pos->X);
					dragPos.Y = (cursorPos.Y - pos->Y);
				}
				pos->X = cursorPos.X - dragPos.X;
				pos->Y = cursorPos.Y - dragPos.Y;

			}
			else
			{
				dragPos = FVector2D{ 0, 0 };

			}
		}
		else
		{
			hover_element = false;
		}


		offset_x = 0.0f; offset_y = 0.0f;
		menu_pos = FVector2D{ pos->X, pos->Y };
		first_element_pos = FVector2D{ 0, 0 };
		current_element_pos = FVector2D{ 0, 0 };
		current_element_size = FVector2D{ 0, 0 };

		drawFilledRect({ pos->X - 2, pos->Y - 2 }, size.X + 4, size.Y + 4, Colors::Window_Outline);
		drawFilledRect(FVector2D{ pos->X, pos->Y }, size.X, size.Y, Colors::Window_Background);
		drawFilledRect(FVector2D{ pos->X, pos->Y }, size.X, 25.0f, Colors::Window_Header);
		Draw_Line({ pos->X, pos->Y + 25.f }, { pos->X + size.X, pos->Y + 25.f }, 2.f, Colors::Window_Line);
		offset_y += 25.0f;

		FVector2D titlePos = FVector2D{ pos->X - 220 + size.X / 2, pos->Y + 10 };
		TextLeft(name, titlePos, Colors::Text, false);

		return true;
	}

	float posz12;
	bool set = false;

	inline void Text(const char* text, bool center = false, bool outline = false, bool a = false)
	{
		elements_count++;

		float size = 25;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (outline) {
			if (!set && a)
			{
				posz12 = pos.Y;
				set = true;
			}
			else
			{
				pos.Y = posz12;
				set = false;
			}

			offset_y = pos.Y - menu_pos.Y + 25.f;
		}
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y - 1.f;
		}

		if (!sameLine && !outline)
			offset_y += size + padding.Y;

		FVector2D textPos = FVector2D{ pos.X + 5.0f, pos.Y + size / 2 };
		if (center)
			TextCenter(text, textPos, Colors::Text, outline);
		else
			TextLeft(text, textPos, Colors::Text, outline);

		sameLine = false;
		last_element_pos = pos;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	inline bool ButtonTab(const char* name, FVector2D size, bool active)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 0, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		auto color = FLinearColor{ 0.0f, 0.0f, 0.0f, 1.0f };

		if (active)
		{
			drawFilledRect(FVector2D{ pos.X + 19, pos.Y - 1 }, size.X + 2, size.Y + 2, color);
			drawFilledRect(FVector2D{ pos.X + 20, pos.Y }, size.X, size.Y, Colors::Button_Active);
		}
		else if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X + 19, pos.Y - 1 }, size.X + 2, size.Y + 2, Colors::Text);
			drawFilledRect(FVector2D{ pos.X + 20, pos.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;

			color = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X + 19, pos.Y - 1 }, size.X + 2, size.Y + 2, Colors::Text);
			drawFilledRect(FVector2D{ pos.X + 20, pos.Y }, size.X, size.Y, Colors::Button_Idle);
			color = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		FVector2D textPos = FVector2D{ pos.X + 15 + size.X / 2, pos.Y - 1 + size.Y / 2 };
		TextCenter(name, textPos, color, false);

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
			return true;

		return false;
	}

	inline bool Button(const char* name, FVector2D size)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		drawFilledRect({ pos.X - 1, pos.Y - 1 }, size.X + 2, size.Y + 2, Colors::Text);
		if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
		TextCenter(name, textPos, Colors::Text, false);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
			return true;

		return false;
	}

	float nigga = 0;

	inline bool Checkbox(const char* name, bool* value)
	{

		elements_count++;

		float size = 14;
		FVector2D padding = FVector2D{ 27, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y + 2.f;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ size, size });

		FVector2D slider_size2 = FVector2D{ 8, 9 };

		drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size2.Y + padding.Y - 19 }, slider_size2.X, slider_size2.Y + 4, *value ? Colors::Checkbox_Idle : Colors::Checkbox_Enabled);
		DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f - 14 }, 5.8f, *value ? Colors::Checkbox_Idle : Colors::Checkbox_Enabled);
		DrawFilledCircle(FVector2D{ pos.X + slider_size2.X, pos.Y + padding.Y + 9.4f - 14 }, 5.8f, *value ? Colors::Checkbox_Idle : Colors::Checkbox_Enabled);

		DrawFilledCircle(FVector2D(pos.X - 15 + (*value ? 23.f : 15.f), pos.Y + 5), 4.9f, *value ? Colors::Checkbox_Enabled : Colors::Checkbox_Enabled);
		DrawCircle(FVector2D(pos.X - 15 + (*value ? 23.f : 15.f), pos.Y + 5), 5.9f, 120, Colors::Checkbox_Idle);

		if (!sameLine)
			offset_y += size + padding.Y;

		FVector2D textPos = FVector2D{ pos.X + size + 5.0f, pos.Y + size / 2 - 2.0f };
		TextLeft(name, textPos, Colors::Text, false);


		sameLine = false;
		last_element_pos = pos;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
			*value = !*value;

		return value;
	}

	inline void SliderInt(const char* name, int* value, int min, int max, int pos2, const char* format = xorstr("%i"))
	{
		elements_count++;

		FVector2D size = FVector2D{ 210, 30 };
		FVector2D slider_size = FVector2D{ 170, 7 };
		FVector2D adjust_zone = FVector2D{ 0, 20 };
		FVector2D padding = FVector2D{ 25, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y - adjust_zone.Y }, FVector2D{ slider_size.X, slider_size.Y + adjust_zone.Y * 1.5f });

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		if (isHovered || current_element == elements_count)
		{
			if (Input::IsMouseClicked(0, elements_count, true))
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos();
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Hovered);
			DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
			DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Idle);
			DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
			DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Idle);
		}

		FVector2D textPos = FVector2D{ pos.X, pos.Y + 5 };
		TextLeft(name, textPos, Colors::Text, false);

		float oneP = slider_size.X / (max - min);
		drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress);
		DrawFilledCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 4.0f, Colors::Slider_Button);
		DrawCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 5.0f, 120, Colors::Slider_Button_Outline);

		char buffer[32];
		sprintf_s(buffer, format, *value);
		FVector2D valuePos = FVector2D{ pos.X + oneP + pos2, pos.Y - 5 + slider_size.Y + 20 + padding.Y };
		TextCenter(buffer, valuePos, Colors::Text, false);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	inline void SliderFloat(const char* name, float* value, float min, float max, int pos2, const char* format = ("%.1f"))
	{
		elements_count++;

		FVector2D size = FVector2D{ 210, 30 };
		FVector2D slider_size = FVector2D{ 170, 7 };
		FVector2D adjust_zone = FVector2D{ 0, 20 };
		FVector2D padding = FVector2D{ 25, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y - adjust_zone.Y }, FVector2D{ slider_size.X, slider_size.Y + adjust_zone.Y * 1.5f });

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		if (isHovered || current_element == elements_count)
		{
			if (Input::IsMouseClicked(0, elements_count, true))
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos();
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Hovered);
			DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
			DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Idle);
			DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
			DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Idle);
		}

		FVector2D textPos = FVector2D{ pos.X, pos.Y + 5 };
		TextLeft(name, textPos, Colors::Text, false);

		float oneP = slider_size.X / (max - min);
		drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress);
		DrawFilledCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 4.0f, Colors::Slider_Button);
		DrawCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 5.0f, 120, Colors::Slider_Button_Outline);

		char buffer[32];
		sprintf_s(buffer, format, *value);
		FVector2D valuePos = FVector2D{ pos.X + oneP + pos2, pos.Y - 5 + slider_size.Y + 20 + padding.Y };
		TextCenter(buffer, valuePos, Colors::Text, false);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	bool checkbox_enabled[256];
	void Combobox(char* name, FVector2D size, int* value, const char* arg, ...)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		//Draw_Line({ pos.X - 1, pos.Y }, { pos.X + size.X + 1, pos.Y + size.Y + 1 }, 2.f, { 1.f, 1.f, 1.f, 1.f });
		drawFilledRect({ pos.X - 1, pos.Y - 1 }, size.X + 2, size.Y + 2, Colors::Text);

		//Bg
		if (isHovered || checkbox_enabled[elements_count])
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Combobox_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Combobox_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + size.X + 5.0f, pos.Y + size.Y / 2 };
		TextLeft(name, textPos, Colors::Text, false);

		//Elements
		bool isHovered2 = false;
		FVector2D element_pos = pos;
		int num = 0;

		if (checkbox_enabled[elements_count])
		{
			current_element_size.X = element_pos.X - 5.0f;
			current_element_size.Y = element_pos.Y - 5.0f;
		}
		va_list arguments;
		for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char*))
		{
			//Selected Element
			if (num == *value)
			{
				FVector2D _textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
				TextCenter((char*)arg, _textPos, Colors::Text, false);
			}

			if (checkbox_enabled[elements_count])
			{
				element_pos.Y += 25.0f;
				drawFilledRect({ element_pos.X - 1.f, element_pos.Y - 1.f }, size.X + 2.f, 27.f, Colors::Text);
				isHovered2 = MouseInZone(FVector2D{ element_pos.X, element_pos.Y }, FVector2D{ size.X, 25.0f });
				if (isHovered2)
				{
					hover_element = true;

					PostRenderer::drawFilledRect(FVector2D{ element_pos.X, element_pos.Y }, size.X, 25.0f, Colors::Combobox_Hovered);

					//Click
					if (Input::IsMouseClicked(0, elements_count, false))
					{
						*value = num;
						checkbox_enabled[elements_count] = false;
					}
				}
				else
				{
					PostRenderer::drawFilledRect(FVector2D{ element_pos.X, element_pos.Y }, size.X, 25.0f, Colors::Combobox_Idle);
				}


				PostRenderer::TextLeft((char*)arg, FVector2D{ element_pos.X + 5.0f, element_pos.Y + 15.0f }, Colors::Text, false);
			}
			num++;
		}
		va_end(arguments);
		if (checkbox_enabled[elements_count])
		{
			current_element_size.X = element_pos.X + 5.0f;
			current_element_size.Y = element_pos.Y + 5.0f;
		}


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && Input::IsMouseClicked(0, elements_count, false))
		{
			checkbox_enabled[elements_count] = !checkbox_enabled[elements_count];
		}
		if (!isHovered && !isHovered2 && Input::IsMouseClicked(0, elements_count, false))
		{
			checkbox_enabled[elements_count] = false;
		}
	}



	std::string VirtualKeyCodeToString(UCHAR virtualKey)
	{
		UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

		if (virtualKey == VK_LBUTTON)
		{
			return xorstr("MOUSE0");
		}
		if (virtualKey == VK_RBUTTON)
		{
			return xorstr("MOUSE1");
		}
		if (virtualKey == VK_MBUTTON)
		{
			return xorstr("MBUTTON");
		}
		if (virtualKey == VK_XBUTTON1)
		{
			return xorstr("XBUTTON1");
		}
		if (virtualKey == VK_XBUTTON2)
		{
			return xorstr("XBUTTON2");
		}

		CHAR szName[128];
		int result = 0;
		switch (virtualKey)
		{
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
		case VK_RCONTROL: case VK_RMENU:
		case VK_LWIN: case VK_RWIN: case VK_APPS:
		case VK_PRIOR: case VK_NEXT:
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE:
		case VK_NUMLOCK:
			scanCode |= KF_EXTENDED;
		default:
			result = GetKeyNameTextA(scanCode << 16, szName, 128);
		}

		return szName;
	}

	void Hotkey(char* name, FVector2D size, int* key)
	{
		static int active_hotkey = -1;
		static bool already_pressed = false;

		elements_count++;

		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y + (last_element_size.Y / 2) - size.Y / 2;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		drawFilledRect(FVector2D{ pos.X - 1.f, pos.Y - 1.f }, size.X + 2.f, size.Y + 2.f, Colors::Text);
		//Bg
		if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		if (active_hotkey == elements_count)
		{
			//Text
			FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
			TextCenter(xorstr("Press a key"), textPos, Colors::Text, false);

			if (!GUI::Input::IsAnyMouseDown())
			{
				already_pressed = false;
			}

			if (!already_pressed)
			{
				for (int code = 0; code < 255; code++)
				{
					if (nt::GetKey(code))
					{
						*key = code;
						active_hotkey = -1;
					}
				}
			}
		}
		else
		{
			//Text
			FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
			TextCenter((char*)VirtualKeyCodeToString(*key).c_str(), textPos, Colors::Text, false);

			if (isHovered)
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{
					already_pressed = true;
					active_hotkey = elements_count;

					//Queue Fix
					for (int code = 0; code < 255; code++)
						if (GetAsyncKeyState(code)) {
						}
				}
			}
			else
			{
				if (Input::IsMouseClicked(0, elements_count, false))
				{
					active_hotkey = -1;
				}
			}
		}


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}

	inline void Render(bool toggle)
	{
		if (!toggle) return;
		for (int i = 0; i < 128; i++)
		{
			if (PostRenderer::drawlist[i].type != -1)
			{
				if (PostRenderer::drawlist[i].type == 1)
				{
					GUI::drawFilledRect(PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].size.X, PostRenderer::drawlist[i].size.Y, PostRenderer::drawlist[i].color);
				}
				else if (PostRenderer::drawlist[i].type == 2)
				{
					GUI::TextLeft(PostRenderer::drawlist[i].name, PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].color, PostRenderer::drawlist[i].outline);
				}
				else if (PostRenderer::drawlist[i].type == 3)
				{
					GUI::TextCenter(PostRenderer::drawlist[i].name, PostRenderer::drawlist[i].pos, PostRenderer::drawlist[i].color, PostRenderer::drawlist[i].outline);
				}
				else if (PostRenderer::drawlist[i].type == 4)
				{
					Draw_Line(PostRenderer::drawlist[i].from, PostRenderer::drawlist[i].to, PostRenderer::drawlist[i].thickness, PostRenderer::drawlist[i].color);
				}

				PostRenderer::drawlist[i].type = -1;
			}
		}
	}
}

void Tick()
{
	GUI::Input::Handle();


	FVector2D pos = GUI::menu_pos;

	static bool menu_opened = false;

	//if(global::Controller->WasInputKeyJustReleased( ) )
	if (nt::GetKey(VK_INSERT) & 1) menu_opened = !menu_opened;

	FVector2D window_size = { 480.0f, 440.0f };
	static int tab = 0;

	if (menu_opened) {

		if (GUI::Window("44444#8245", &pos, window_size, menu_opened))
		{
			if (GUI::ButtonTab(xorstr("Aimbot"), FVector2D{ 147, 22 }, tab == 0))
				tab = 0;
			GUI::SameLine();
			if (GUI::ButtonTab(xorstr("Visuals"), FVector2D{ 147, 22 }, tab == 1))
				tab = 1;
			GUI::SameLine();
			if (GUI::ButtonTab(xorstr("Misc"), FVector2D{ 147, 22 }, tab == 2))
				tab = 2;

			if (tab == 0)
			{

				GUI::Checkbox(xorstr("Enable Aimbot"), &Settings::Setings.Aimbot.Aimbot);
				GUI::Text(xorstr("Aimbot mode"));
				GUI::NextColumn(10.f);
				GUI::Checkbox(xorstr("Memory"), &Settings::Setings.Aimbot.Memory);
				GUI::Checkbox(xorstr("Silent"), &Settings::Setings.Aimbot.Silent);
				GUI::Checkbox(xorstr("Enable visibility check"), &Settings::Setings.Aimbot.visible);
				std::string bone = xorstr("Current bone: ");
				bone += Engine::Aimbot::g[Settings::Setings.Aimbot.aimbone].first;
				bone += xorstr(". Change by PAGEUP/PAGEDOWN");
				GUI::Text(bone.c_str());// GUI::SameLine();
				GUI::Combobox(xorstr(""), { 100.f, 25.f }, &Settings::Setings.Aimbot.aimbone, xorstr("Legs"), xorstr("Head"), xorstr("Torso"), NULL);

				GUI::Checkbox(xorstr("Draw Field of view"), &Settings::Setings.Aimbot.draw);
				GUI::SliderFloat(xorstr("Field of View"), &Settings::Setings.Aimbot.FieldOV, 10.f, 1000.f, 150);
				GUI::SliderFloat(xorstr("Smoothing"), &Settings::Setings.Aimbot.Smoothing, 0.f, 50.f, 150);

				GUI::Hotkey(xorstr("Aimbot key"), { 80, 25 }, &Settings::Setings.Aimbot.aimkey);
			}
			else if (tab == 1)
			{
				GUI::Checkbox(xorstr("Player skeleton"), &Settings::Setings.Visuals.Skeleton);
				GUI::Checkbox(xorstr("Player head circle"), &Settings::Setings.Visuals.headcircle);
				GUI::Checkbox(xorstr("Player box esp"), &Settings::Setings.Visuals.Boxesp);
				GUI::Checkbox(xorstr("Player snaplines"), &Settings::Setings.Visuals.snaplines);
				GUI::Checkbox(xorstr("Player name"), &Settings::Setings.Visuals.name);
				GUI::Checkbox(xorstr("Player distance"), &Settings::Setings.Visuals.distance);
				GUI::Checkbox(xorstr("Player weapon"), &Settings::Setings.Visuals.weapon);
				GUI::Checkbox(xorstr("Draw self"), &Settings::Setings.Visuals.draw_self);

			}
			else if (tab == 2)
			{
				GUI::Checkbox(xorstr("No recoil"), &Settings::Setings.Exploits.norecoil);
				GUI::Checkbox(xorstr("No spread"), &Settings::Setings.Exploits.nospread);
				GUI::Checkbox(xorstr("Double pump"), &Settings::Setings.Exploits.nosway);
				GUI::Checkbox(xorstr("Rapid fire"), &Settings::Setings.Exploits.rapidfire);
				GUI::Checkbox(xorstr("Custom Time Dil"), &Settings::customtime);
				GUI::Checkbox(xorstr("Insta reload"), &Settings::Setings.Exploits.instareload);
				GUI::Checkbox(xorstr("Vehicle fly"), &Settings::Setings.Exploits.fly);
				GUI::SliderFloat(xorstr("Fly speed"), &Settings::dilation, 0.1f, 100.f, 150);
				GUI::Checkbox(xorstr("Projectile TP"), &Settings::Setings.Exploits.projectiletp);
				GUI::Checkbox(xorstr("Draw info"), &Settings::Setings.Exploits.debug);
				GUI::Checkbox(xorstr("Crosshair"), &Settings::Setings.Exploits.crosshair);
				GUI::Checkbox(xorstr("FOV changer"), &Settings::Setings.Exploits.fovchanger);
				GUI::SliderFloat(xorstr("FOV"), &Settings::Setings.Exploits.FOV, 30.f, 170.f, 150);
				GUI::SliderFloat(xorstr("FOV"), &Settings::Setings.Exploits.rapid_fire_time, 1.f, 70.f, 150);
			}
		}

		GUI::Render(menu_opened);
		GUI::Draw_Cursor(menu_opened);
	}
}

inline void draw_line(FVector2D a, FVector2D b, FLinearColor clr, float thicc)
{
	Engine::K2_DrawLine(GUI::canvas, a, b, thicc, clr);
}

inline void draw_text(const char* text, FVector2D a, FLinearColor clr, bool fag = false)
{
	Engine::K2_DrawText(GUI::canvas, FString{ s2wc(text) }, a, { 1.f, 1.f }, clr, 0, { 0.f, 0.f, 0.f, 0.f }, { 0, 0 }, fag, false, true, Colors::Black);
}

inline void wdraw_text(wchar_t* text, FVector2D a, FLinearColor clr, bool fag = false)
{
	Engine::K2_DrawText(GUI::canvas, FString{ text }, a, { 1.f, 1.f }, clr, 0, { 0.f, 0.f, 0.f, 0.f }, { 0, 0 }, fag, false, true, Colors::Black);
}

inline void CalculateBox(uintptr_t mesh, FName* array, int size, FVector2D& topleft, FVector2D& downright)
{
	for (int i = 0; i <= size; i++)
	{
		FVector bone = Engine::GetSocketLocation(mesh, array[i]);

		if (bone == FVector())
			continue;

		FVector2D x = Engine::WorldToScreen(bone);

		if (x.X > downright.X)
			downright.X = x.X;

		if (x.Y > downright.Y)
			downright.Y = x.Y;

		if (x.X < topleft.X)
			topleft.X = x.X;

		if (x.Y < topleft.Y)
			topleft.Y = x.Y;
	}
}