#include "Input.h"

bool Input::isPressed(int key) { return Application::getInstance().getWindow().getKey(key); }
