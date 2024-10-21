#pragma once

#include "RendererAPI.h"

namespace Graphite {

    class OpenGLRendererAPI : public RendererAPI {
    public:
        virtual void Init() override {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override {
            glViewport(x, y, width, height);
        }

        virtual void Clear() override {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        virtual void SetClearColor(float r, float g, float b, float a) override {
            glClearColor(r, g, b, a);
        }

        virtual void DrawIndexed(const VertexArray& vertexArray, unsigned int indexCount = 0) override {
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        }
    };

}
