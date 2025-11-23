//
// Created by cvnpko on 11/20/25.
//
#include <app/MainController.hpp>
#include <random>

namespace engine::main::app {
void MainPlatformEventObserver::on_key(engine::platform::Key key) {
    spdlog::info("Keyboard event: key={}, state={}", key.name(), key.state_str());
}

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    spdlog::info("MousePosition: {} {}", position.x, position.y);
}

void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->camera()->Position = {-45, 1, 0};
    graphics->camera()->MovementSpeed *= 4.0f;

    auto observer = std::make_unique<MainPlatformEventObserver>();
    engine::core::Controller::get<engine::platform::PlatformController>()->register_platform_event_observer(
            std::move(observer));

    m_trees.push_back({{-35.0f, 0.0f, 40.0f}, {2.6f, 2.6f, 2.6f}});
    m_trees.push_back({{0.0f, 0.0f, 34.0f}, {3.0f, 3.0f, 3.0f}});
    m_trees.push_back({{-40.0f, 0.0f, -45.0f}, {2.2f, 2.2f, 2.2f}});
    m_trees.push_back({{-5.0f, 0.0f, -35.0f}, {2.8f, 2.8f, 2.8f}});
    m_trees.push_back({{15.0f, 0.0f, -40.0f}, {3.2f, 3.2f, 3.2f}});
    m_trees.push_back({{38.0f, 0.0f, -35.0f}, {3.20f, 3.20f, 3.20f}});
    m_trees.push_back({{38.0f, 0.0f, -10.0f}, {2.6f, 2.6f, 2.6f}});
    m_trees.push_back({{38.0f, 0.0f, 30.0f}, {3.8f, 3.8f, 3.8f}});
    m_trees.push_back({{36.0f, 0.0f, 8.0f}, {2.6f, 2.6f, 2.6f}});
    m_trees.push_back({{20.0f, 0.0f, 25.0f}, {2.8f, 2.8f, 2.8f}});
    m_trees.push_back({{15.0f, 0.0f, 40.0f}, {2.6f, 2.6f, 2.6f}});

    std::random_device rd;
    std::mt19937 gen(rd());
    {
        std::uniform_real_distribution<float> distx(-49.9f, -18.1f);
        std::uniform_real_distribution<float> disty(-0.01f, 0.01f);
        std::uniform_real_distribution<float> distz(-49.9f, 49.9f);
        std::uniform_real_distribution<float> dists(0.8f, 1.2f);
        std::uniform_real_distribution<float> dista(0.0f, 360.0f);
        glm::vec3 vecn = {0.0f, 1.0f, 0.0f};
        for (int i = 0; i < 10000; i++) {
            m_grass.push_back({{distx(gen), disty(gen), distz(gen)},
                               {dists(gen), dists(gen), dists(gen)}});
            m_grass.back().Rotate.emplace_back(vecn, dista(gen));
        }
    }
    {
        std::uniform_real_distribution<float> distx(-12.0f, 49.9f);
        std::uniform_real_distribution<float> disty(-0.01f, 0.01f);
        std::uniform_real_distribution<float> distz(-49.9f, 49.9f);
        std::uniform_real_distribution<float> dists(0.8f, 1.2f);
        std::uniform_real_distribution<float> dista(0.0f, 360.0f);
        glm::vec3 vecn = {0.0f, 1.0f, 0.0f};
        for (int i = 0; i < 10000; i++) {
            m_grass.push_back({{distx(gen), disty(gen), distz(gen)},
                               {dists(gen), dists(gen), dists(gen)}});
            m_grass.back().Rotate.emplace_back(vecn, dista(gen));
        }
    }
}

bool MainController::loop() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE)
                .state() == engine::platform::Key::State::JustPressed) {
        return false;
    }
    return true;
}

void MainController::poll_events() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_F1)
                .state() == engine::platform::Key::State::JustPressed) {
        m_cursor_enabled = !m_cursor_enabled;
        platform->set_enable_cursor(m_cursor_enabled);
    }
}

void MainController::update() {
    update_camera();
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw() {
    draw_grass();
    draw_floor();
    draw_tree();
    draw_castle();
}

void MainController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

void MainController::draw_grass() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
    auto grass = engine::core::Controller::get<engine::resources::ResourcesController>()->model("grass");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    for (auto &g: m_grass) {
        shader->set_mat4("model", get_model_matrix(g));
        grass->draw(shader);
    }
}


void MainController::draw_floor() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
    auto floor = engine::core::Controller::get<engine::resources::ResourcesController>()->model("floor");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_mat4("model", get_model_matrix(m_floor));
    floor->draw(shader);
}

void MainController::draw_tree() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
    auto pine_tree = engine::core::Controller::get<engine::resources::ResourcesController>()->model("pine_tree");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    for (auto &t: m_trees) {
        shader->set_mat4("model", get_model_matrix(t));
        pine_tree->draw(shader);
    }
}

void MainController::draw_castle() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
    auto castle = engine::core::Controller::get<engine::resources::ResourcesController>()->model("castle");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_mat4("model", get_model_matrix(m_castle));
    castle->draw(shader);
}

glm::mat4 MainController::get_model_matrix(ModelParams par) const {
    glm::mat4 ret = scale(translate(glm::mat4(1.0f), par.Position), par.Scale);
    for (auto &r: par.Rotate) {
        ret = rotate(ret, glm::radians(r.second), r.first);
    }
    return ret;
}


void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    float dt = platform->dt();
    if (platform->key(engine::platform::KEY_W)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }
    if (platform->key(engine::platform::KEY_S)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }
    if (platform->key(engine::platform::KEY_A)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }
    if (platform->key(engine::platform::KEY_D)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
    auto mouse = platform->mouse();
    camera->rotate_camera(mouse.dx, mouse.dy);
    camera->zoom(mouse.scroll);
}
}// namespace engine::main::app
