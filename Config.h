#pragma once

// Hằng số dùng chung cho toàn dự án (tránh hard-code rải rác).
namespace Config
{
constexpr unsigned int WINDOW_WIDTH = 960;
constexpr unsigned int WINDOW_HEIGHT = 540;

constexpr float TILE = 32.f;

constexpr float PLAYER_WIDTH = 28.f;
constexpr float PLAYER_HEIGHT = 44.f;

// Vật lý chạy theo bước thời gian cố định -> va chạm ổn định, không phụ thuộc FPS.
constexpr float FIXED_STEP = 1.f / 120.f;
constexpr float MAX_FRAME_TIME = 0.25f;
}
