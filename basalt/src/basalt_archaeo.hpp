#pragma once
#include <string>
#include <vector>
#include <memory>
#include "sdl2_plat.hpp"
#include "basalt.h"

constexpr int MAX_DRAW_CALLS = 128;

enum ArchaeoState { ARCHAEO_IDLE, ARCHAEO_RECORDING, ARCHAEO_OPENED };

struct DrawCall {
    std::string description;
    Texture texture;
};

class Archaeo {
    ArchaeoState state;
    int selectedIndex;

    Color* canvasPixels;
    uint drawCallCount;

    std::vector<DrawCall> drawCalls;

   private:
    static std::shared_ptr<Archaeo> instance;
    void RegisterDrawCallImpl(Texture canvas, std::string description);
    bool UpdateAndRenderImpl(Texture canvas);

   public:
    static void RegisterDrawCall(Texture canvas, std::string description);
    static bool UpdateAndRender(Texture canvas);
};