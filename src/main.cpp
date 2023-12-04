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
#include "ArithmeticCode.h"

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
	auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
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
	ArithmeticCode *code;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
		ImGui::Begin("Find Interval", &show_upload_button,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
					 ImGuiWindowFlags_NoResize);


		if (filepath.empty()) {

			auto text = "Welcome to the Arithmetic Interval Finder";
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
			if (!file.is_open()) {
				upload_success = false;
				continue;
			}
			upload_success = true;

			std::string line;
			std::getline(file, line);
			auto num_lines = std::strtoul(line.c_str(), nullptr, 10);


			auto text = "00 Inputs";
			auto textSize = ImGui::CalcTextSize(text);
			ImGui::PushFont(fontLg);
			AlignForWidth(textSize.x, 0.05f);
			AlignForHeight(textSize.y, 0.05f);

			ImGui::Text("%lu Inputs", num_lines);
			ImGui::PopFont();
			ImGui::Dummy(ImVec2(0, 10));

			code = new ArithmeticCode[num_lines];

			for (auto i = 0; i < num_lines; i++) {
				std::getline(file, line);
				code[i].input = line;
				code[i].a = 0;
				code[i].b = 1;
				encode(code[i]);


				std::stringstream stream;
				stream << std::fixed << std::setprecision(15)
					   << code[i].a << std::endl
					   << code[i].b << std::endl;

				std::string a;
				std::getline(stream, a);
				std::string b;
				std::getline(stream, b);

				std::string res;
				res.append("Input: ");
				res.append(code[i].input);
				res.append("\nInterval: [");
				res.append(a);
				res.append(", ");
				res.append(b);
				res.append(")");

				AlignForWidth(0, 0.1f);
				ImGui::Text("%s", res.c_str());

				ImGui::Dummy(ImVec2(0, 10));
			}

			text = "Back";
			textSize = ImGui::CalcTextSize(text);
			AlignForHeight(textSize.y);
			AlignForWidth(textSize.x, 0.5f);
			if (ImGui::Button("Back")) {
				delete[] code;
				filepath = "";
			}
			ImGui::Dummy(ImVec2(0, 50));
		}


		ImGui::End();

		// Rendering
		ImGui::Render();

		SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255),
							   (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
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
