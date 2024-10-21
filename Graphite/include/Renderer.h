#pragma once
#include "RendererAPI.h"

namespace Graphite {

    class Renderer {
    public:
        static void Init() {
            s_RendererAPI->Init();
        }

        static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        static void Clear() {
            s_RendererAPI->Clear();
        }

        static void SetClearColor(float r, float g, float b, float a) {
            s_RendererAPI->SetClearColor(r, g, b, a);
        }

        static void DrawIndexed(const VertexArray& vertexArray, unsigned int indexCount = 0) {
            s_RendererAPI->DrawIndexed(vertexArray, indexCount);
        }

        static void SetRendererAPI(RendererAPI* rendererAPI) {
            s_RendererAPI = rendererAPI;
        }

    private:
        static RendererAPI* s_RendererAPI;
    };

}

Graphite::Renderer::Init();
