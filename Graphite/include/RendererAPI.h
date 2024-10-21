// RendererAPI.h
#pragma once



namespace Graphite {

    class RendererAPI {
    public:
        virtual void Init() = 0;
        virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
        virtual void Clear() = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void DrawIndexed(const class VertexArray& vertexArray, unsigned int indexCount = 0) = 0;
        virtual ~RendererAPI() = default;
    };

}
