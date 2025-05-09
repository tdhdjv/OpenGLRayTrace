#pragma once
#include <GLM/glm.hpp>
#include <GLM/vec3.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera();

	void setPosition(glm::vec3 position);
	void setViewDir(glm::vec3 viewDir);
	void setYaw(float value);
	void setPitch(float value);
	void setRoll(float value);

	inline const glm::vec3& getPosition() const { return position; };
	inline const glm::vec3& getViewDir() const { return viewDir; };
	inline const glm::mat4& getViewMat() const { return viewMat; };
	inline const glm::mat4& getProjectionMat() const { return projectionMat; }

	inline glm::vec3 getFront() { return front; };
	inline glm::vec3 getRight() { return right; };

	inline float getYaw() { return yaw; };
	inline float getPitch() { return pitch; };
	inline float getRoll() { return roll; };
private:

	glm::vec3 position;
	glm::vec3 viewDir;
	glm::mat4 viewMat;
	const glm::mat4 projectionMat;

	const glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 front;
	glm::vec3 right;

	float yaw;
	float pitch;
	float roll;
};