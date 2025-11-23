
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

void Model::draw(const Shader *shader) {
    shader->use();
    for (auto &mesh: m_meshes) {
        mesh.draw(shader);
    }
}

void Model::draw_instancing(const Shader *shader, uint32_t amount) {
    shader->use();
    for (auto &mesh: m_meshes) {
        mesh.draw_instancing(shader, amount);
    }
}

void Model::destroy() {
    for (auto &mesh: m_meshes) {
        mesh.destroy();
    }
}
}// namespace engine::resources
