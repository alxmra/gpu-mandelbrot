#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Shader.h"

int screen_width { 720 };
int screen_height { 720 };

int num_frames{ 0 };
float last_time{ 0.0f };

float vertices[] = {
    -1.0f,  -1.0f,  -0.0f,
     1.0f,   1.0f,  -0.0f,
    -1.0f,   1.0f,  -0.0f,
     1.0f, -1.0f, -0.0f
};

unsigned int indices[] = {
    0, 1, 2,
    0, 3, 1
};

float center_x{ -0.15f };
float center_y{ 0.0f };
float zoom{ 1.0 };

float increment{ 0.0f };

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
        increment += 0.00001f;
    }

    if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS) {
        increment -= 0.00001f;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        center_y = center_y + 0.005f * zoom;
        if (center_y > 1.0f)
            center_y = 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        center_y = center_y - 0.005f * zoom;
        if (center_y < -1.0f)
            center_y = -1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        center_x = center_x - 0.005f * zoom;
        if (center_x < -1.0f)
        {
            center_x = -1.0f;
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        center_x = center_x + 0.005f * zoom;
        if (center_x > 1.0f)
        {
            center_x = 1.0f;
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        zoom = zoom * 1.02f;
        if (zoom > 1.0f)
        {
            zoom = 1.0f;
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        zoom = zoom * 0.98f;
        if (zoom < 0.00001f)
        {
            zoom = 0.00001f;
        }
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void countFPS() {
    double current_time = glfwGetTime();
    num_frames++;
    if (current_time - last_time >= 1.0) {
        std::cout << 1000.0 / num_frames << "ms / frame\n";
        num_frames = 0;
        last_time += 1.0;
    }
}

glm::vec4 find_ranges(std::vector<float> &data) {
    std::sort(data.begin(), data.end());
    int lowest = 0;
    while (data[lowest] == 0.0f)
        ++lowest;

    int size = data.size();
    int length = size - lowest;
    glm::vec4 ranges = glm::vec4( data[lowest], data[lowest + length * 3 / 4 - 1], data[lowest + length * 7 / 8 - 1], data[size - 1]);
    return ranges;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Mandelgoat", NULL, NULL);

    if (window == nullptr) {
        std::cout << "Failed to create a GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit())
        std::cout << "Failed to initialize GLEW.\n";

    glViewport(0, 0, screen_width, screen_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    last_time = glfwGetTime();

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    Shader shader("shader.vert", "shader.frag");

    std::vector<float> pixel_data(screen_width * screen_height, 0.0f);
    glm::vec4 ranges = glm::vec4(0.0001f, 0.33333f, 0.66667f, 1.00f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        countFPS();

        process_input(window);

        shader.use_shader();
        shader.set_float("increment", increment);
        shader.set_float("zoom", zoom);
        shader.set_float("center_x", center_x);
        shader.set_float("center_y", center_y);
        shader.set_vec4("color_ranges", ranges);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glReadPixels(0, 0, screen_width, screen_height, GL_DEPTH_COMPONENT, GL_FLOAT, pixel_data.data());
        ranges = find_ranges(pixel_data);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
