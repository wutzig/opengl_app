#pragma once
const int INIT_WIDTH = 800;
const int INIT_HEIGHT = 600;
const int SHADOW_WIDTH = glm::pow(2, 12);
const int SHADOW_HEIGHT = glm::pow(2, 12);
const int NUM_GRID = 19;
const int NUM_TREES = 60;
const float OFFSET = 0.05f;
const float TILE_SIZE = 1.0f;
const float PI = 3.14159;
const float INIT_CAMERA_YAW = 0.0f;
const float INIT_CAMERA_PITCH = -PI / 6.0f;

const float dark_green[12] = {0.4f, 0.5f, 0.2f, 0.4f, 0.5f, 0.2f, 0.4f, 0.5f, 0.2f, 0.4f, 0.5f, 0.2f};
const float light_green[12] = {0.6f, 0.7f, 0.3f, 0.6f, 0.7f, 0.3f, 0.6f, 0.7f, 0.3f, 0.6f, 0.7f, 0.3f};
const float light_blue[12] = {0.4f, 0.4f, 0.8f, 0.4f, 0.4f, 0.8f, 0.4f, 0.4f, 0.8f, 0.4f, 0.4f, 0.8f};