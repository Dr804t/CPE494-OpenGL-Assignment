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
//#include <learnopengl/model.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>

// callbacks and helpers
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
bool CheckAABBCollision(const glm::vec3& posA, const glm::vec3& sizeA,
    const glm::vec3& posB, const glm::vec3& sizeB);

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // positions          // normals           // texcoords
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 18.0f));
Camera camera(glm::vec3(0.0f, 10.0f, 0.0f),  // position
    glm::vec3(0.0f, 1.0f, 0.0f),   // world up
    -90.0f,                        // yaw (look along -Z)
    -89.9f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// model position
glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, -8.0f);
bool isMoving = false;
float modelYaw = 180.0f;

struct Obstacle {
    glm::vec3 position;
    glm::vec3 size;
};

std::vector<Obstacle> obstacles = {
    { glm::vec3(3.0f, 1.0f, 3.0f), glm::vec3(2.0f, 2.0f, 2.0f) },
    { glm::vec3(-5.0f, 1.5f, -2.0f), glm::vec3(3.0f, 3.0f, 3.0f) },
    { glm::vec3(0.0f, 1.0f, -8.0f), glm::vec3(4.0f, 2.0f, 1.0f) }
};

float worldSpeed = 5.0f;         // initial speed
float maxWorldSpeed = 20.0f;     // cap speed
float speedIncreaseRate = 0.5f;  // units per second

float spawnZ = -50.0f;           // where new obstacles spawn
float obstacleSpacing = 10.0f;   // distance between obstacles
float laneOffset = 2.5f;         // X distance for lanes
int numObstacles = 10;           // pool size

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
    stbi_set_flip_vertically_on_load(true);

    // build and compile shaders
    Shader lightingShader("6.multiple_lights.vs", "6.multiple_lights.fs");
    Shader lightCubeShader("6.light_cube.vs", "6.light_cube.fs");
	Shader animShader("anim_model.vs", "anim_model.fs");

    float planeVertices[] = {
        // positions            // normals         // texcoords
        // Ground plane in XZ
        25.0f, 0.0f,  25.0f,   0.0f, 1.0f, 0.0f,   25.0f,  0.0f,
       -25.0f, 0.0f,  25.0f,   0.0f, 1.0f, 0.0f,    0.0f,  0.0f,
       -25.0f, 0.0f, -25.0f,   0.0f, 1.0f, 0.0f,    0.0f, 25.0f,

        25.0f, 0.0f,  25.0f,   0.0f, 1.0f, 0.0f,   25.0f,  0.0f,
       -25.0f, 0.0f, -25.0f,   0.0f, 1.0f, 0.0f,    0.0f, 25.0f,
        25.0f, 0.0f, -25.0f,   0.0f, 1.0f, 0.0f,   25.0f, 25.0f
    };

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

    // load models real animation
    // -----------
    Model ourModel(FileSystem::getPath("resources/objects/Player/Player.dae"));
	Animation running(FileSystem::getPath("resources/objects/Player/Player.dae"), &ourModel);
    Animation idle(FileSystem::getPath("resources/objects/Player/idle/idle.dae"), &ourModel);
    Animator animator(&idle);
    Animation* currentAnimation = &idle;

    //example animation
   /* Model ourModel(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"));
    Animation danceAnimation(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"), &ourModel);
    Animator animator(&danceAnimation);*/

    std::vector<Obstacle> obstacles;
    for (int i = 0; i < numObstacles; i++)
    {
        float z = -i * obstacleSpacing - 10.0f;
        float x = ((i % 3) - 1) * laneOffset; // left, center, right
        obstacles.push_back({ glm::vec3(x, 1.0f, z), glm::vec3(2.0f, 2.0f, 2.0f) });
    }


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // gradually increase world speed over time
        worldSpeed += speedIncreaseRate * deltaTime;

        // cap the speed to avoid going too fast
        if (worldSpeed > maxWorldSpeed)
            worldSpeed = maxWorldSpeed;


        // input
        processInput(window);
        if (isMoving && currentAnimation != &running)
        {
            animator.PlayAnimation(&running);
            currentAnimation = &running;
        }
        else if (!isMoving && currentAnimation != &idle)
        {
            animator.PlayAnimation(&idle);
            currentAnimation = &idle;
        }

        animator.UpdateAnimation(deltaTime);


        // clear
        glClearColor(0.83f, 0.03f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // inside render loop, after glClear(...)

        // activate shader
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 64.0f);

        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, 1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
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
            lightingShader.setVec3(base + ".diffuse", 1.0f * pulse, 1.0f * pulse, 1.0f * pulse); // slightly tinted
            lightingShader.setVec3(base + ".specular", 1.0f * pulse, 1.0f * pulse, 1.0f * pulse);
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
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glBindVertexArray(planeVAO);

        glm::mat4 model = glm::mat4(1.0f); // already positioned in world space
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // bind diffuse texture (e.g. obsidian)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glm::vec3 playerSize(0.5f, 1.0f, 0.5f);


        for (auto& obs : obstacles)
        {

            obs.position.z += worldSpeed * deltaTime;

            // Recycle obstacle when it passes the player
            if (obs.position.z > 5.0f)
            {
                obs.position.z = spawnZ;
                obs.position.x = ((rand() % 3) - 1) * laneOffset; // random lane
            }
            if (CheckAABBCollision(modelPosition, playerSize, obs.position, obs.size))
            {
                std::cout << "💥 Game Over!" << std::endl;
                glfwSetWindowShouldClose(window, true);
                break;
            }
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obs.position);
            model = glm::scale(model, obs.size * 0.5f); // match collider dimensions
            lightingShader.setMat4("model", model);

            // You can use a cube VAO — if you don't have one, create it similarly to your plane VAO.
            renderCube(); // helper that draws a cube
        }

		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
		
        
        auto transforms = animator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            animShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, modelPosition);

        // rotate to face the movement direction (around Y axis)
        model = glm::rotate(model, glm::radians(modelYaw), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::scale(model, glm::vec3(0.5f));
        animShader.setMat4("model", model);
        ourModel.Draw(animShader);

        // Define a fixed camera offset (behind and above the player)
        glm::vec3 cameraOffset(0.0f, 7.0f, 10.0f); // 10 up, 15 behind

        // Keep the camera behind the player (in world space, not rotating with yaw)
        camera.Position = modelPosition + cameraOffset;

        // Always look toward the player
        camera.Front = glm::normalize(modelPosition - camera.Position);

        // swap buffers / poll IO
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &planeVAO);

    // terminate
    glfwTerminate();
    return 0;
}

// process input
void processInput(GLFWwindow* window)
{
    isMoving = true;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // Model movement (arrow keys)
    float speed = 1.0f * deltaTime;
    glm::vec3 moveDir(0.0f);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        modelPosition.z -= speed;
		moveDir.z -= 1.0f;
        /*isMoving = true;*/
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        modelPosition.z += speed;
        moveDir.z += 1.0f;
        /*isMoving = true;*/
    }
    /*if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        modelPosition.x -= speed;
        moveDir.x -= 1.0f;
        isMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        modelPosition.x += speed;
        moveDir.x += 1.0f;
        isMoving = true;
    }*/
    float laneSpeed = 6.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        modelPosition.x = glm::max(modelPosition.x - laneSpeed, -laneOffset);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        modelPosition.x = glm::min(modelPosition.x + laneSpeed, laneOffset);
    static bool isJumping = false;
    static float jumpVelocity = 0.0f;
    static float gravity = -9.8f;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping) {
        isJumping = true;
        jumpVelocity = 5.0f;
    }
    if (isJumping) {
        modelPosition.y += jumpVelocity * deltaTime;
        jumpVelocity += gravity * deltaTime;
        if (modelPosition.y <= 0.0f) {
            modelPosition.y = 0.0f;
            isJumping = false;
        }
    }


    if (glm::length(moveDir) > 0.0f)
    {
        moveDir = glm::normalize(moveDir);

        glm::vec3 proposedPos = modelPosition + moveDir * speed;
        glm::vec3 playerSize(1.0f, 2.0f, 1.0f);
        bool collision = false;

        for (const auto& obs : obstacles)
        {
            if (CheckAABBCollision(modelPosition, playerSize, obs.position, obs.size))
            {
                glm::vec3 diff = modelPosition - obs.position;
                glm::vec3 overlap = (obs.size + playerSize) * 0.5f - glm::abs(diff);

                // resolve smallest overlap axis
                if (overlap.x < overlap.z)
                    modelPosition.x += (diff.x > 0 ? overlap.x : -overlap.x);
                else
                    modelPosition.z += (diff.z > 0 ? overlap.z : -overlap.z);
				collision = true;
				printf("Collision detected!\n");
				break;
            }
        }

        if (!collision)
            modelPosition = proposedPos;

        // rotate to face movement direction
        modelYaw = glm::degrees(atan2(moveDir.x, moveDir.z));
    }

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

bool CheckAABBCollision(const glm::vec3& posA, const glm::vec3& sizeA,
    const glm::vec3& posB, const glm::vec3& sizeB)
{
    glm::vec3 halfA = sizeA * 0.5f;
    glm::vec3 halfB = sizeB * 0.5f;

    bool overlapX = posA.x + halfA.x > posB.x - halfB.x &&
        posA.x - halfA.x < posB.x + halfB.x;
    bool overlapY = posA.y + halfA.y > posB.y - halfB.y &&
        posA.y - halfA.y < posB.y + halfB.y;
    bool overlapZ = posA.z + halfA.z > posB.z - halfB.z &&
        posA.z - halfA.z < posB.z + halfB.z;

    return overlapX && overlapY && overlapZ;
}
