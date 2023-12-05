#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <tinyfiledialogs.h>
#include <string>
#include <fstream>
#include <sstream>

#include "myfont.cpp"
#include "style.h"
#include "DCT.h"

#define WIDTH 1200
#define HEIGHT 800


// Main code
int main(int, char **) {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0) {
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	// Enable native IME.
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	// Create window with SDL_Renderer graphics context
	auto window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
	SDL_Window *window = SDL_CreateWindow("", WIDTH, HEIGHT, window_flags);
	if (window == nullptr) {
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
		return -1;
	}

	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);
	SDL_SetWindowMinimumSize(window, 600, 600);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Platform/Renderer backends for SDL
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
	ImGui::StyleColorsDark();
	//
	ImFont *font = io.Fonts->AddFontFromMemoryCompressedTTF(MyFont_compressed_data, MyFont_compressed_size, 18.0f);
	ImFont *fontLg = io.Fonts->AddFontFromMemoryCompressedTTF(MyFont_compressed_data, MyFont_compressed_size, 26.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 10));
	ImGui::GetStyle().FrameRounding = 10;

	IM_ASSERT(font != nullptr);
	// Our state
	static bool show_upload_button = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool done = false;
	bool upload_success = true;
	std::string filepath;
	DCT dct;
	while (!done) {
		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;

			break;
		}

		ImGui::NewFrame();

		int x, y;
		SDL_GetWindowSize(window, &x, &y);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(x, y));
		ImGui::Begin("Find Interval", &show_upload_button,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
					 ImGuiWindowFlags_NoResize);


		if (filepath.empty()) {

			auto text = "Welcome to Discrete Cosine Transformer";
			ImGui::PushFont(fontLg);
			auto textSize = ImGui::CalcTextSize(text);
			AlignForWidth(textSize.x, 0.5);
			AlignForHeight(textSize.y, 0.25f);
			ImGui::Text("%s", text);
			ImGui::PopFont();


			text = "Upload File to Continue";
			textSize = ImGui::CalcTextSize(text);
			AlignForWidth(textSize.x);
			AlignForHeight(textSize.y);


			if (ImGui::Button("Upload File to Continue")) {
				char *f = tinyfd_openFileDialog(
						"Select a .txt file to display",
						"",
						1,
						(const char *[]) {"*.txt"},
						"Text files",
						0);

				if (f) {
					filepath = f;
				} else {
					upload_success = false;
				}
			}

			if (!upload_success) {
				text = "File Upload Failed... Try again";
				textSize = ImGui::CalcTextSize(text);
				AlignForWidth(textSize.x);
				ImGui::Text("%s", text);
			}


			text = "Close";
			textSize = ImGui::CalcTextSize(text);
			ImGui::Dummy(ImVec2(0, 10));
			AlignForWidth(textSize.x);
			if (ImGui::Button("Close")) {
				ImGui::End();
				ImGui::EndFrame();
				break;
			}


		} else {

			std::ifstream file(filepath);

			// Check for file open errors, and tracks the upload state in a boolean
			if (!file.is_open()) {
				upload_success = false;
				continue;
			}
			upload_success = true;


			// Parse the input only once, unless the user wants to upload a new file
			if (!dct.is_parsed) {
				std::string line;
				std::getline(file, line);
				auto num_lines = std::strtoul(line.c_str(), nullptr, 10);

				dct.size = num_lines;
				dct.input.resize(num_lines);
				char delim = ' ';
				for (int i = 0; i < dct.size; i++) {
					dct.input[i].resize(dct.size);
					std::getline(file, line);

					for (int j = 0; j < dct.size; j++) {
						std::string token = line.substr(0, line.find(delim));
						dct.input[i][j] = std::strtoul(token.c_str(), nullptr, 10);
						line.erase(0, line.find(delim) + 1);
					}
				}
				dct.set_row_major();
				dct.set_col_major();
				dct.is_parsed = true;
			}

			// Display the Input Matrix
			print_matrix(window, "Input Matrix", dct.input);
			print_matrix(window, "Row Major", dct.row_major);
			print_matrix(window, "Col Major", dct.col_major);


			auto text = std::string("Back");
			auto textSize = ImGui::CalcTextSize(text.c_str());
			ImGui::Dummy(ImVec2(0, 50));
			AlignForHeight(textSize.y);
			AlignForWidth(textSize.x, 0.5f);
			if (ImGui::Button("Back")) {
				filepath = "";
				dct.is_parsed = false;
			}
			ImGui::Dummy(ImVec2(0, 50));
		}


		ImGui::End();

		// Rendering
		ImGui::Render();

		SDL_SetRenderDrawColor(renderer, (Uint8) (clear_color.x * 255), (Uint8) (clear_color.y * 255),
							   (Uint8) (clear_color.z * 255), (Uint8) (clear_color.w * 255));
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}
	ImGui::PopStyleVar();

	// Cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

