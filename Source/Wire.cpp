#include "Wire.h"

Wire::Wire(glm::vec3 x_0, glm::vec3 x_1) {

	Vert.push_back(x_0.x);
	Vert.push_back(x_0.y);
	Vert.push_back(x_0.z);

	Vert.push_back(x_1.x);
	Vert.push_back(x_1.y);
	Vert.push_back(x_1.z);

	Ind.push_back(0);
	Ind.push_back(1);
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vert.size() * sizeof(float), Vert.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Ind.size() * sizeof(GLuint), Ind.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

};

Wire::~Wire() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Wire::Draw(Shader& shader, Camera& camera,
	glm::vec3& translation,
	glm::quat& rotation,
	glm::vec3& scale) {
	shader.Activate();
	glBindVertexArray(VAO);

	camera.Matrix(90.0f, 0.1f, 1000.0f, shader, "camMatrix");

	// Transform the matrices to their correct form //ifi
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rot = glm::mat4_cast(rotation);
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));

	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}