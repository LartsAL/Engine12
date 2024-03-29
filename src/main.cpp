#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SystemVars.h"
#include "FileManager.h"
#include "Shader.h"
#include "Texture.h"

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

float mixKoef = 0;
// position.x * cos(time) - position.y * sin(time), position.x * sin(time) + position.y * cos(time)

int main(int argc, char* argv[]) {
    // Инициализация GLFW
    glfwInit();

    // Настройка GLFW
    // Требуемая версия GLFW: 3.3. Формат: major.minor
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    SystemVars::openGLVersionMajor = 3;
    SystemVars::openGLVersionMinor = 3;

    // Установка профайла, для которого создаётся контекст ?
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Запрет изменения размеров окна
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    // Создание объекта окна
    GLFWwindow* window = glfwCreateWindow(800, 800, "Engine12 Sandbox", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // Установка контекста
    glfwMakeContextCurrent(window);


    // Регистрация функции обратного вызова для проверки нажатия клавиш
    glfwSetKeyCallback(window, key_callback);

    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // Установка размера отображаемого окна ?
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);



    // Массив вершин
    GLfloat vertices[] = {
            // Вершины         // Цвета           // Текстура
           -0.5, -0.5,  0.0,   1.0,  0.0,  0.0,   0.0,  0.0,   // Левый нижний
           -0.5,  0.5,  0.0,   0.0,  1.0,  0.0,   0.0,  1.0,   // Левый верхний
            0.5,  0.5,  0.0,   0.0,  0.0,  1.0,   1.0,  1.0,   // Правый верхний
            0.5, -0.5,  0.0,   1.0,  1.0,  1.0,   1.0,  0.0    // Правый нижний
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));

    // Включаем атрибут с индексом N
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

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



    Shader myShader = FileManager::loadShader("resources/shaders/vertex.vert", "resources/shaders/fragment.frag");

    Texture myTexture1("resources/textures/wooden_crate.jpg");
    Texture myTexture2("resources/textures/emergency_exit.png");

    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        // Проверка событий
        glfwPollEvents();

        // Очистка буфера
        glClearColor(0.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);


        // Рисуем
        myShader.use();

        //float time = glfwGetTime();
        //GLint uniformTimeLocation = glGetUniformLocation(myShader.programID, "_time");
        //glUniform1f(uniformTimeLocation, -time);
        glUniform1f(glGetUniformLocation(myShader.programID, "mixKoef"), mixKoef);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, myTexture1.textureID);
        glUniform1i(glGetUniformLocation(myShader.programID, "myTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, myTexture2.textureID);
        glUniform1i(glGetUniformLocation(myShader.programID, "myTexture2"), 1);


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // Переключение буферов
        glfwSwapBuffers(window);
    }

    // Освобождение ресурсов
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        mixKoef = clamp(mixKoef+0.02f, 0.0f, 1.0f);
    }

    if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        mixKoef = clamp(mixKoef-0.02f, 0.0f, 1.0f);
    }
}
