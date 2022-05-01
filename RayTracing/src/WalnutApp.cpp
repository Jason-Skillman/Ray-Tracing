#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
private:
	std::shared_ptr<Image> image;
	uint32_t* imageData = nullptr;
	uint32_t viewportWidth = 0, viewportHeight = 0;
	float lastRenderTimer = 0;

public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		{
			ImGui::Text("Last render: %.3f ms", lastRenderTimer);

			if(ImGui::Button("Render")) {
				Render();
			}
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");
		{
			viewportWidth = ImGui::GetContentRegionAvail().x;
			viewportHeight = ImGui::GetContentRegionAvail().y;

			if(image)
				ImGui::Image(image->GetDescriptorSet(), { static_cast<float>(image->GetWidth()), static_cast<float>(image->GetHeight()) });
		}
		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

private:
	void Render() {
		Timer timer;

		uint32_t viewportSize = viewportWidth * viewportHeight;

		if(!image || viewportWidth != image->GetWidth() || viewportHeight != image->GetHeight()) {

			//RGBA is 4 bytes (1 byte for each channel)
			image = std::make_shared<Image>(viewportWidth, viewportHeight, ImageFormat::RGBA);

			delete[] imageData;
			imageData = new uint32_t[viewportSize];
		}

		for(uint32_t i = 0; i < viewportSize; i++) {
			//AABBGGRR
			//imageData[i] = 0xffff00ff;
			//imageData[i] = 0xffed9564;
			imageData[i] = Random::UInt();
			imageData[i] |= 0xff000000;	//Exclude the alpha channel from random
		}

		image->SetData(imageData);

		lastRenderTimer = timer.ElapsedMillis();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}