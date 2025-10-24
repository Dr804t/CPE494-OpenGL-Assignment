// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include <string>
#include <vector>
#include <learnopengl/model.h>

// callbacks and helpers
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 18.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Kinetic Sculpture - Floating Helical Cubes", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // callbacks and input mode
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(false);

    // build and compile shaders
    Shader lightingShader("6.multiple_lights.vs", "6.multiple_lights.fs");
    Shader lightCubeShader("6.light_cube.vs", "6.light_cube.fs");

    // vertex data (positions, normals, texcoords)
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float planeVertices[] = {
        // positions            // normals         // texcoords
        // Ground plane in XZ
        25.0f, -7.0f,  25.0f,   0.0f, 1.0f, 0.0f,   25.0f,  0.0f,
       -25.0f, -7.0f,  25.0f,   0.0f, 1.0f, 0.0f,    0.0f,  0.0f,
       -25.0f, -7.0f, -25.0f,   0.0f, 1.0f, 0.0f,    0.0f, 25.0f,
    
        25.0f, -7.0f,  25.0f,   0.0f, 1.0f, 0.0f,   25.0f,  0.0f,
       -25.0f, -7.0f, -25.0f,   0.0f, 1.0f, 0.0f,    0.0f, 25.0f,
        25.0f, -7.0f, -25.0f,   0.0f, 1.0f, 0.0f,   25.0f, 25.0f
    };


    // buffers / arrays
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light cube VAO (reuses VBO)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // textures
    unsigned int diffuseMap = loadTexture(FileSystem::getPath("resources/textures/obsidian.png").c_str());
    unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/block_solid.png").c_str());
	unsigned int floorTexture = loadTexture(FileSystem::getPath("resources/textures/endstone.jpg").c_str());
    // shader config
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    // point lights (static positions for visual anchors)
    glm::vec3 pointLightPositions[] = {
        glm::vec3(3.0f,  1.5f,  4.0f),
        glm::vec3(-3.0f, -2.5f, -4.0f),
        glm::vec3(5.0f,  2.0f, -8.0f),
        glm::vec3(-4.0f,  2.0f,  2.0f)
    };

    // -----------------------------------------------------
    // Generate a full cylinder mesh (sides + caps) with EBO
    // -----------------------------------------------------
    unsigned int cylinderVAO, cylinderVBO, cylinderEBO;
    std::vector<float> cylinderVertices;
    std::vector<unsigned int> cylinderIndices;

    const int sectorCount = 32;   // number of radial segments
    const float radius = 5.0f;
    const float height = 15.0f;

    // --- 1️⃣ Build side vertices ---
    for (int i = 0; i <= sectorCount; ++i) {
        float theta = (float)i / sectorCount * glm::two_pi<float>();
        float x = cos(theta);
        float z = sin(theta);
        float u = (float)i / sectorCount;

        // bottom vertex
        cylinderVertices.push_back(radius * x);
        cylinderVertices.push_back(-height / 2.0f);
        cylinderVertices.push_back(radius * z);
        cylinderVertices.push_back(x); cylinderVertices.push_back(0.0f); cylinderVertices.push_back(z);
        cylinderVertices.push_back(u); cylinderVertices.push_back(0.0f);

        // top vertex
        cylinderVertices.push_back(radius * x);
        cylinderVertices.push_back(height / 2.0f);
        cylinderVertices.push_back(radius * z);
        cylinderVertices.push_back(x); cylinderVertices.push_back(0.0f); cylinderVertices.push_back(z);
        cylinderVertices.push_back(u); cylinderVertices.push_back(1.0f);
    }

    // --- 2️⃣ Build side indices ---
    for (int i = 0; i < sectorCount; ++i) {
        unsigned int bottom1 = i * 2;
        unsigned int top1 = bottom1 + 1;
        unsigned int bottom2 = bottom1 + 2;
        unsigned int top2 = bottom2 + 1;

        // first triangle
        cylinderIndices.push_back(bottom1);
        cylinderIndices.push_back(top1);
        cylinderIndices.push_back(bottom2);

        // second triangle
        cylinderIndices.push_back(top1);
        cylinderIndices.push_back(top2);
        cylinderIndices.push_back(bottom2);
    }

    // --- 3️⃣ Build top and bottom caps ---
    unsigned int baseVertexCount = (sectorCount + 1) * 2;

    // bottom center vertex
    unsigned int bottomCenterIndex = baseVertexCount;
    cylinderVertices.push_back(0.0f);
    cylinderVertices.push_back(-height / 2.0f);
    cylinderVertices.push_back(0.0f);
    cylinderVertices.push_back(0.0f); cylinderVertices.push_back(-1.0f); cylinderVertices.push_back(0.0f);
    cylinderVertices.push_back(0.5f); cylinderVertices.push_back(0.5f);

    // top center vertex
    unsigned int topCenterIndex = baseVertexCount + 1;
    cylinderVertices.push_back(0.0f);
    cylinderVertices.push_back(height / 2.0f);
    cylinderVertices.push_back(0.0f);
    cylinderVertices.push_back(0.0f); cylinderVertices.push_back(1.0f); cylinderVertices.push_back(0.0f);
    cylinderVertices.push_back(0.5f); cylinderVertices.push_back(0.5f);

    // bottom cap indices
    for (int i = 0; i < sectorCount; ++i) {
        unsigned int curr = i * 2;
        unsigned int next = (i + 1) * 2;
        cylinderIndices.push_back(bottomCenterIndex);
        cylinderIndices.push_back(next);
        cylinderIndices.push_back(curr);
    }

    // top cap indices
    for (int i = 0; i < sectorCount; ++i) {
        unsigned int curr = i * 2 + 1;
        unsigned int next = (i + 1) * 2 + 1;
        cylinderIndices.push_back(topCenterIndex);
        cylinderIndices.push_back(curr);
        cylinderIndices.push_back(next);
    }

    // --- 4️⃣ Upload to GPU ---
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glGenBuffers(1, &cylinderEBO);

    glBindVertexArray(cylinderVAO);

    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(float),
        cylinderVertices.data(), GL_STATIC_DRAW);

    // element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(unsigned int),
        cylinderIndices.data(), GL_STATIC_DRAW);

    // vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    std::printf("Cylinder: %zu vertices, %zu indices\n",
        cylinderVertices.size() / 8, cylinderIndices.size());


    // Cylinder positions around the center (like a circle)
    std::vector<glm::vec3> cylinderPositions = {
        glm::vec3(15.0f, -7.0f,  0.0f),
        glm::vec3(-15.0f, -7.0f,  0.0f),
        glm::vec3(-7.0f, -7.0f,  15.0f),
        glm::vec3(0.0f, -7.0f, -15.0f),
        glm::vec3(14.0f, -7.0f,  14.0f)
    };

    // --- background setup (insert after your existing shader config) ---
    Shader backgroundShader("6.background.vs", "6.background.fs");

    // fullscreen quad (NDC) with texcoords
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 1.0f
    };

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    // load background texture (replace path with your image)
    unsigned int bgTexture = loadTexture(FileSystem::getPath("resources/textures/endportal.png").c_str());

    // tell shader which unit
    backgroundShader.use();
    backgroundShader.setInt("bgTexture", 0);
    // --- end background setup ---

    // load models
    // -----------
    Model ourModel(FileSystem::getPath("resources/objects/enderdragon/scene.obj"));


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // clear
        glClearColor(0.03f, 0.03f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // inside render loop, after glClear(...)
    // draw background
        glDisable(GL_DEPTH_TEST); // draw background without depth testing
        backgroundShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bgTexture);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST); // restore depth testing

        // then continue with your existing scene rendering (lightingShader, objects, etc.)

        // activate shader
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 64.0f);

        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.15f, 0.15f, 0.15f);
        lightingShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);


        // point lights (we slightly pulse their diffuse color over time for life)
        for (int i = 0; i < 4; ++i)
        {
            std::string base = "pointLights[" + std::to_string(i) + "]";
            glm::vec3 pos = pointLightPositions[i];
            lightingShader.setVec3(base + ".position", pos);

            // pulsing factor (0.75 to 1.0)
            float pulse = 0.75f + 0.25f * sin(currentFrame * 2.0f + float(i));
            lightingShader.setVec3(base + ".ambient", 0.02f * pulse, 0.02f * pulse, 0.02f * pulse);
            lightingShader.setVec3(base + ".diffuse", 1.0f * pulse, 0.0f * pulse, 1.0f * pulse); // slightly tinted
            lightingShader.setVec3(base + ".specular", 1.0f * pulse, 0.0f * pulse, 1.0f * pulse);
            lightingShader.setFloat(base + ".constant", 1.0f);
            lightingShader.setFloat(base + ".linear", 0.09f);
            lightingShader.setFloat(base + ".quadratic", 0.032f);
        }

        // spotlight (attached to the camera)
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

        // view / projection matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render animated helix of cubes
        glBindVertexArray(cubeVAO);

        const unsigned int cubeCount = 16;      // number of cubes in the helix
        float radius = 6.0f;                    // helix radius
        float verticalSpacing = 0.6f;           // how far along Y each step moves
        float helixTurns = 2.0f;                // how many turns the helix spans
        float speed = 1.0f;                     // global animation speed

        for (unsigned int i = 0; i < cubeCount; ++i)
        {
            // parameter t per-cube
            float idx = (float)i / (float)cubeCount;
            float baseAngle = idx * helixTurns * glm::two_pi<float>(); // spread around helix
            float timeOffset = currentFrame * speed + idx * 1.5f;

            // position in helix with additional sin wave for organic motion
            float angle = baseAngle + timeOffset * 0.6f;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            float y = (idx - 0.5f) * cubeCount * verticalSpacing + sin(timeOffset * 1.5f + idx * 2.0f) * 0.8f;

            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(x, y, z));

            // each cube rotates differently (mix axis)
            glm::vec3 rotAxis = glm::normalize(glm::vec3(0.4f + idx, 1.0f - idx, 0.6f));
            float rotSpeed = 0.8f + 1.2f * sin(timeOffset * 0.7f + idx * 3.14f);
            model = glm::rotate(model, timeOffset * rotSpeed, rotAxis);

            // pulsating scale for "breathing" effect
            float scale = 0.6f + 0.4f * (0.5f + 0.5f * sin(timeOffset * 2.0f + idx * 2.0f));
            model = glm::scale(model, glm::vec3(scale));

            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(planeVAO);

        glm::mat4 model = glm::mat4(1.0f); // already positioned in world space
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(cylinderVAO);

        // Use same texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Draw 5 cylinders in a circle
        for (const auto& pos : cylinderPositions)
        {
            glm::mat4 cylindermodel = glm::mat4(1.0f);
            cylindermodel = glm::translate(cylindermodel, pos);

            // Make them slightly taller/thinner for variety
            cylindermodel = glm::scale(cylindermodel, glm::vec3(0.8f, 2.0f, 0.8f));

            lightingShader.setMat4("model", cylindermodel);
            glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
        }

        // draw the lamp objects (point lights)
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; ++i)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.25f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // reset texture bindings so the model loader can bind its own textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        lightingShader.use();

        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        ourModel.Draw(lightingShader);

        // swap buffers / poll IO
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &VBO);

    // terminate
    glfwTerminate();
    return 0;
}

// process input
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

// framebuffer resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// mouse movement
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// texture loader
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    // stb_image flips by default? keep as in your project; here we don't call stbi_set_flip_vertically_on_load
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << "\n";
        stbi_image_free(data);
    }

    return textureID;
}
