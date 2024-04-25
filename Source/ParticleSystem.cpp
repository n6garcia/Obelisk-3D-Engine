#include "ParticleSystem.h"

const double PI = 3.1415926535897932384626433832795028841971693993751058209;

ParticleSystem::ParticleSystem(){

    Vert.push_back(1.0f);
    Vert.push_back(1.0f);
    Vert.push_back(0.0f);

    Vert.push_back(0.0f);
    Vert.push_back(0.0f);

    Vert.push_back(-1.0f);
    Vert.push_back(1.0f);
    Vert.push_back(0.0f);

    Vert.push_back(1.0f);
    Vert.push_back(0.0f);

    Vert.push_back(-1.0f);
    Vert.push_back(-1.0f);
    Vert.push_back(0.0f);

    Vert.push_back(1.0f);
    Vert.push_back(1.0f);

    Vert.push_back(1.0f);
    Vert.push_back(-1.0f);
    Vert.push_back(0.0f);

    Vert.push_back(0.0f);
    Vert.push_back(1.0f);   

    Ind.push_back(0);
    Ind.push_back(1);
    Ind.push_back(2);

    Ind.push_back(0);
    Ind.push_back(2);
    Ind.push_back(3);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vert.size() * sizeof(float), Vert.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Ind.size() * sizeof(GLuint), Ind.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 *sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    sprite = new Texture("models/smoke/smoke.png", "diffuse", 0);

}

void ParticleSystem::DrawParticles(Shader& shader, Camera& camera){
    shader.Activate();
	glBindVertexArray(VAO);

	sprite->texUnit(shader, "sprite");
	sprite->Bind();

	camera.Matrix(shader, "camMatrix");

    transforms.clear();

    for (int i = 0; i < particles.size(); i++){
        glm::vec3 vec2cam = camera.getPosition() - particles[i].getTranslation();
        vec2cam.y = 0.f;
        vec2cam = glm::normalize(vec2cam);

        glm::vec3 orientation = glm::vec3(0.0f,0.0f,1.0f);
        orientation = glm::normalize(orientation);

        glm::vec3 crossProd = glm::cross(orientation, vec2cam);
        float w = sqrt(powf(glm::length(orientation),2)*powf(glm::length(vec2cam), 2)) + glm::dot(orientation, vec2cam);

        glm::quat rotation = glm::quat(w,crossProd);

        float len = glm::length(rotation);

        rotation.x = rotation.x / len;
        rotation.y = rotation.y / len;
        rotation.z = rotation.z / len;
        rotation.w = rotation.w / len;

        glm::vec3 vec2cam2 = camera.getPosition() - particles[i].getTranslation();
        vec2cam2 = glm::normalize(vec2cam2);

        crossProd = glm::cross(vec2cam, vec2cam2);
        w = sqrt(powf(glm::length(vec2cam), 2) * powf(glm::length(vec2cam2), 2)) + glm::dot(vec2cam, vec2cam2);

        glm::quat rotation2 = glm::quat(w, crossProd);

        len = glm::length(rotation2);

        rotation2.x = rotation2.x / len;
        rotation2.y = rotation2.y / len;
        rotation2.z = rotation2.z / len;
        rotation2.w = rotation2.w / len;

        float theta = particles[i].rotMag * 2.0f * PI/particles[i].expire;
        float q1 = cos(theta / 2.0f);
        float q2 = vec2cam2.x * sin(theta / 2.0f);
        float q3 = vec2cam2.y * sin(theta / 2.0f);
        float q4 = vec2cam2.z * sin(theta / 2.0f);
        glm::quat rotation3 = glm::quat(q1, q2, q3, q4);

        glm::quat rotCombine = rotation3 * rotation2 * rotation;

        // Transform the matrices to their correct form
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), particles[i].getTranslation());
        glm::mat4 rot = glm::mat4_cast(rotCombine);
        glm::mat4 sca = glm::scale(glm::mat4(1.0f), particles[i].scale);

        transforms.push_back(trans * rot * sca);
    }

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));

	glUniform1f(glGetUniformLocation(shader.ID, "life"), life);

	glDrawElementsInstanced(GL_TRIANGLES, Ind.size(), GL_UNSIGNED_INT, 0, particles.size());

}