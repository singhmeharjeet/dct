//
// Created by Meharjeet Singh on 2023-11-03.
//

#ifndef IMAGE_STYLE_H
#define IMAGE_STYLE_H

#ifndef WIDTH
#define WIDTH 1200
#endif

#include <imgui.h>

#include <cstdio>


void AlignForWidth(float width, float alignment = 0.5f) {
	ImGuiStyle &style = ImGui::GetStyle();
	float avail = ImGui::GetContentRegionAvail().x;
	float off = (avail - width) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

void AlignForHeight(float height, float alignment = 0.5f) {
	ImGuiStyle &style = ImGui::GetStyle();
	float avail = ImGui::GetContentRegionAvail().y;
	float off = (avail - height) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off);
}

#endif //IMAGE_STYLE_H