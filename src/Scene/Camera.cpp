#include "Camera.h"

Camera::Camera()
	:position(0.0), viewDir(0.0, 0.0, 1.0), viewMat(glm::lookAt(position, position + viewDir, up)),
	yaw(0.0), pitch(0.0), roll(0.0) 
{
	right = glm::normalize(glm::cross(viewDir, up));
	front = glm::cross(up, right);
}

void Camera::setPosition(glm::vec3 position) {
	this->position = position;
	viewMat = glm::lookAt(position, position + viewDir, up);
}

void Camera::setViewDir(glm:: vec3 viewDir) {
	this->viewDir = viewDir;
	right = glm::normalize(glm::cross(viewDir, up));
	front = glm::cross(up, right);
	viewMat = glm::lookAt(position, position + viewDir, up);
}

void Camera::setYaw(float value) {
	yaw = value;
	setViewDir(glm::vec3(glm::cos(pitch) * glm::sin(yaw), glm::sin(pitch), glm::cos(pitch) * glm::cos(yaw)));
}

void Camera::setPitch(float value) {
	pitch = value;
	setViewDir(glm::vec3(glm::cos(pitch) * glm::sin(yaw), glm::sin(pitch), glm::cos(pitch) * glm::cos(yaw)));
}

void Camera::setRoll(float value) {
	roll = value;
}

