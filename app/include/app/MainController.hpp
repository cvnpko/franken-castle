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
    struct ModelParams {
        glm::vec3 Position;
        glm::vec3 Scale;
        std::vector<std::pair<glm::vec3, float>> Rotate;
    };

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

    void draw_castle();

    void draw_grass();

    void draw_bridge();

    static glm::mat4 get_model_matrix(ModelParams par);

    ModelParams m_floor{{0.0f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.1f}};
    ModelParams m_castle{{1.5f, -0.6f, 0.0f}, {0.12f, 0.12f, 0.12f}};
    ModelParams m_bridge{{-11.2f, 0.0f, -4.8f}, {5.0f, 4.8f, 8.0f}};
    std::vector<glm::mat4> m_trees;
    std::vector<glm::mat4> m_grass;
    bool m_cursor_enabled{true};
};
}// namespace engine::main::app
#endif//MAINCONTROLLER_HPP
