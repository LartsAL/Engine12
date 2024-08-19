#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "components/Shader.h"
#include "components/Texture.h"
//#include "managers/FileManager.h"
//#include "managers/InputManager.h"
//#include "managers/WindowManager.h"
//#include "managers/SceneManager.h"
#include <Core.h>

using namespace std;

auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) -> void;
auto mouse_callback(GLFWwindow* window, double xpos, double ypos) -> void;
auto scroll_callback(GLFWwindow* window, double xoffset, double yoffset) -> void;

auto cameraMovement() -> void;
auto textureMixing() -> void;


float mixKoef = 0;
// position.x * cos(time) - position.y * sin(time), position.x * sin(time) + position.y * cos(time)

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastMouseX = WIDTH/2;
GLfloat lastMouseY = HEIGHT/2;

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat fov = 45.0f;

int main(int argc, char* argv[]) {
    // # Testing
    Core& core = Core::getInstance();
    core.initialize();

    FileManager& fileManager = FileManager::getInstance();
    WindowManager& windowManager = WindowManager::getInstance();
    SceneManager& sceneManager = SceneManager::getInstance();


    // Инициализация GLFW
    glfwInit();

    // Настройка GLFW
    // Требуемая версия GLFW: 3.3. Формат: major.minor
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Установка профайла, для которого создаётся контекст ?
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Запрет изменения размеров окна
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // # Testing
    auto scene1 = sceneManager.createScene();
    auto window1 = windowManager.createWindow(scene1, 600, 400);


    // Создание объекта окна
    GLFWwindow* window;
    window = glfwCreateWindow(WIDTH, HEIGHT, "Engine12 Sandbox", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // Установка контекста
    glfwMakeContextCurrent(window);

    // Захват и скрытие курсора
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Регистрация функций обратного вызова для нажатий клавиш и движения мыши
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetCursorPos(window, lastMouseX, lastMouseY);

    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);



    // Массив вершин
    GLfloat vertices[] = {
             // Вершины           // Текстура
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    GLuint indices[] = {
            0, 1, 2,
            0, 2, 3
    };


    // Что делать, если текстурные координаты вышли за пределы
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    // Выбор фильтрации текстур
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // Генерация VBO, VAO, EBO
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Привязываем VAO
    glBindVertexArray(VAO);

    // Привязка буффера ?
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Копирование данных в буфер в формате (куда, сколько, кого, как часто меняются данные)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // По факту говорим, как читать данные из массива
    // Аргументы:
    /*
    #1 (0) - какой аргумент шейдера настраиваем. Здесь настраиваем position,
    у которой поставили layout (location = 0), поэтому пишем ноль
    #2 (3) - размер аргумента в шейдере. 3, потому что использовали vec3
    #3 (GL_FLOAT) - какой тип данных используется
    #4 (GL_FALSE) - нормализовывать ли данные
    #5 (3 * sizeof(GLfloat)) - шаг между наборами данных
    #6 ((GLvoid*) 0) - оффсет начала данных в буфере
    */

    // Вершины, цвета, текстуры
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));

    // Включаем атрибут с индексом N
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);

    // Отвязка буффера ?
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // Возможно это вообще нельзя отвязывать..

    // Отвязываем VAO
    glBindVertexArray(0);


    // Общий вид отрисовки объекта:
    /*
    // 0. Копируем массив с вершинами в буфер OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 1. Затем установим указатели на вершинные атрибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // 2. Используем нашу шейдерную программу
    glUseProgram(shaderProgram);
    // 3. Теперь уже отрисовываем объект
    someOpenGlFunctionThatDrawsOutTriangle();
    */

    // Включаем проверку глубины
    glEnable(GL_DEPTH_TEST);


    Shader myShader(fileManager.loadShader("resources/shaders/vertex.vert", "resources/shaders/fragment.frag"));

    Texture myTexture1(fileManager.loadTexture("resources/textures/wooden_crate.jpg"));
    Texture myTexture2(fileManager.loadTexture("resources/textures/emergency_exit.png"));

    GLint mixKoefLocation = glGetUniformLocation(myShader.shaderProgramID, "mixKoef");

    GLint modelLocation = glGetUniformLocation(myShader.shaderProgramID, "model");
    GLint viewLocation = glGetUniformLocation(myShader.shaderProgramID, "view");
    GLint projectionLocation = glGetUniformLocation(myShader.shaderProgramID, "projection");


//    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//
//    glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);  // UP in global coords
//    glm::vec3 cameraRight = glm::normalize(glm::cross(globalUp, cameraDirection));  // Local camera RIGHT
//    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);



    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        // # Testing
        core.update();

        glfwMakeContextCurrent(window);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Проверка событий
        glfwPollEvents();

        if (keys[GLFW_KEY_ESCAPE]) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        cameraMovement();
        textureMixing();

        // Очистка буфера
        glClearColor(0.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Рисуем
        glUseProgram(myShader.shaderProgramID);

        //float time = glfwGetTime();
        //GLint uniformTimeLocation = glGetUniformLocation(myShader.programID, "_time");
        //glUniform1f(uniformTimeLocation, -time);

        glUniform1f(mixKoefLocation, mixKoef);

        //glm::mat4 model(1.0f);
        //model = glm::rotate(model, glm::radians((GLfloat) glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 1.0f));

        //glm::mat4 view(1.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // cameraPos + cameraTarget + cameraUp
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);



        glm::mat4 projection(1.0f);

        projection = glm::perspective(glm::radians(fov) /*glm::radians(45.0f)*/, (GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, myTexture1.textureID);
        glUniform1i(glGetUniformLocation(myShader.shaderProgramID, "myTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, myTexture2.textureID);
        glUniform1i(glGetUniformLocation(myShader.shaderProgramID, "myTexture2"), 1);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);

        for (GLuint i = 0; i < 10; ++i) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, glm::radians((GLfloat) glfwGetTime() * angle), glm::vec3(1.0f, 0.3f, 0.5));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);



        // Переключение буферов
        glfwSwapBuffers(window);
    }

    // # Testing
    core.shutdown();

    // Освобождение ресурсов
    glfwTerminate();

    return 0;
}

auto cameraMovement() -> void {
    GLfloat cameraSpeed = 5.0f * deltaTime;

    if (keys[GLFW_KEY_W]) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_S]) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_A]) {
        cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (keys[GLFW_KEY_D]) {
        cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
}

auto textureMixing() -> void {
    GLfloat mixSpeed = 0.75f;
    if (keys[GLFW_KEY_KP_ADD]) {
        mixKoef = clamp(mixKoef + mixSpeed * deltaTime, 0.0f, 1.0f);
    }
    if (keys[GLFW_KEY_KP_SUBTRACT]) {
        mixKoef = clamp(mixKoef - mixSpeed * deltaTime, 0.0f, 1.0f);
    }
}

auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) -> void {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

auto mouse_callback(GLFWwindow* window, double xpos, double ypos) -> void {
    GLfloat sensitivity = 0.05f;

    GLfloat xoffset = xpos - lastMouseX;
    GLfloat yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    pitch = clamp(pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

auto scroll_callback(GLFWwindow* window, double xoffset, double yoffset) -> void {
    fov = clamp(fov - (GLfloat) yoffset, 1.0f, 45.0f);
}
