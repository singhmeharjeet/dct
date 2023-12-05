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


template<typename T>
void print_matrix(SDL_Window *window, const std::string &name, const std::vector<std::vector<T>> &matrix) {
	const std::string &text = name;
	ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
	ImGui::Dummy(ImVec2(0, 50));
	AlignForWidth(textSize.x, 0.05f);
	ImGui::Text("%s", text.c_str());
	ImGui::PopFont();
	ImGui::Dummy(ImVec2(0, 10));

	int x;
	SDL_GetWindowSize(window, &x, nullptr);

	int num_cols = matrix.size() + 4;
	if (x < 900) {
		num_cols = matrix.size() + 1;
	} else if (x < 1000) {
		num_cols = matrix.size() + 2;
	}


	if (ImGui::BeginTable(name.c_str(), num_cols)) {
		auto offset = 1; // Used for alignment
		for (size_t row = 0; row < matrix.size(); row++) {
			ImGui::TableNextRow();
			for (size_t column = offset; column - offset < matrix.size(); column++) {
				ImGui::TableSetColumnIndex(column);
				if (typeid(matrix[row][column - offset]) == typeid(int))
					ImGui::Text("%   d", matrix[row][column - offset]);
				else {
					ImGui::Text("%.2f", matrix[row][column - offset]);
				}
			}
		}
		ImGui::EndTable();
	}
}

#endif //IMAGE_STYLE_H