//
// Created by cvnpko on 11/20/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <memory>
#include <spdlog/spdlog.h>
namespace engine::main::app {
class MainPlatformEventObserver final : public engine::platform::PlatformEventObserver {
public:
    void on_key(engine::platform::Key key) override;

    void on_mouse_move(engine::platform::MousePosition position) override;
};

class MainController final : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "main::app::MainController";
    }

private:
    void initialize() override;

    bool loop() override;

    void poll_events() override;

    void update() override;

    void begin_draw() override;

    void draw() override;

    void end_draw() override;

    void update_camera();

    void draw_floor();

    void draw_tree();

    glm::vec3 m_floor_position{400.0f, -10.0f, 0.0f};
    float m_floor_scale{0.01f};
    glm::vec3 m_tree_position{0.0f, 0.0f, 0.0f};
    float m_tree_scale{0.1f};
    bool m_cursor_enabled{true};
};
}// namespace engine::main::app
#endif//MAINCONTROLLER_HPP
