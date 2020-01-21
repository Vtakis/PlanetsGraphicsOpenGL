#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 25.0f));
float lastX = 800.0 / 2.0;
float lastY = 600.0 / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float sunAngle = 0.0f;
float rotateSun = 0.0005f;

float moonAngle = 0.0f;
float rotateMoon = 0.0001f;

float earthAngle = 0.0f;
float rotateEarth = 0.01f;

glm::mat4 modelEarth;
glm::mat4 modelMoon;

bool stop = false;
// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SolarSystem", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback); //key SPACE press

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader planetShader("lighting_maps.vs", "lighting_maps.fs");
    Shader sunShader("lamp.vs", "lamp.fs");

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    planetShader.use();
    planetShader.setMat4("projection", projection);

    planetShader.use();
    planetShader.setInt("material.diffuse", 0);
    planetShader.setInt("material.specular", 1);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    Model Earth(FileSystem::getPath("resources/earth/Model/Globe.obj"));
    //Model Moon(FileSystem::getPath("resources/rock/rock.obj"));
    Model Moon(FileSystem::getPath("resources/planet/planet.obj"));
    Model Sun(FileSystem::getPath("resources/planet/planet.obj"));

    float currentFrame;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        planetShader.use();
        planetShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        planetShader.setVec3("lightColor", 0.5f, 0.5f, 0.5f);
        planetShader.setVec3("lightPos", glm::vec3(0.0, 0.0, 3.0));

        // light properties of earth
        planetShader.setVec3("light.ambient", 0.05f, 0.05f, 0.05f);
        planetShader.setVec3("light.diffuse", 3.5f, 3.5f, 3.5f);
        planetShader.setVec3("light.specular", 0.01f, 0.01f, 0.01f);

        planetShader.setFloat("material.shininess", 1.0f);

        // view/projection transformations
        planetShader.use();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);

        glm::mat4 model;
        unsigned int modelLoc;
        if (stop == true) {
            planetShader.setMat4("model", modelEarth);
            Earth.Draw(planetShader);
        }
        else {
            // Earth
            model = glm::mat4(1.0f);
            planetShader.setMat4("model", model);
            model = glm::translate(model, glm::vec3(0.0, 0.0, 3.0));
            model = glm::rotate(model, sunAngle, glm::vec3(0.0f, 1.0, 0.0));
            sunAngle += rotateSun;
            if (sunAngle > 360.0)
                sunAngle -= 360.0;
            modelLoc = glGetUniformLocation(planetShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            model = glm::translate(model, glm::vec3(15.0, 0.0, 3.0));
            model = glm::rotate(model, earthAngle, glm::vec3(0.0f, -1.0, 0.0));
            earthAngle += rotateEarth;
            if (earthAngle > 360.0)
                earthAngle -= 360.0;
            model = glm::scale(model, glm::vec3(0.2f));
            modelEarth = model;
            planetShader.setMat4("model", model);

            Earth.Draw(planetShader);
        }

        // light properties of moon
        planetShader.setVec3("light.ambient", 0.0f, 0.0f, 0.0f);
        planetShader.setVec3("light.diffuse", 0.64f, 0.64, 0.64);
        planetShader.setVec3("light.specular", 0.087302f, 0.087302f, 0.087302f);
        planetShader.setFloat("material.shininess", 1.0f);

        if (stop == true) {
            planetShader.setMat4("model", modelMoon);
            Moon.Draw(planetShader);
        }
        else {
            //moon
            model = modelEarth;
            planetShader.setMat4("model", model);
            model = glm::rotate(model, moonAngle, glm::vec3(0.0f, -1.0, 0.0));
            model = glm::translate(model, glm::vec3(27.0, 0.0, 3.0));
            moonAngle += rotateMoon;
            if (moonAngle > 360.0)
                moonAngle -= 360.0;
            modelLoc = glGetUniformLocation(planetShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            model = glm::scale(model, glm::vec3(1.0f));
            planetShader.setMat4("model", model);
            modelMoon = model;
            Moon.Draw(planetShader);
        }

        // Sun
        sunShader.use();
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0, 3.0));
        model = glm::scale(model, glm::vec3(1.0f));
        sunShader.setMat4("model", model);

        Sun.Draw(sunShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the SPACE key is pressed, value of global boolean variable is changing
// ---------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        stop = !stop;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}