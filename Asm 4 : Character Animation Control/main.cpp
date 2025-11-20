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

// ======== PLAYER CLASS ==========
class Player {
public:
    glm::vec3 position;
    glm::vec3 size;
    float yaw;

    bool isMoving;
    glm::vec3 velocity;
    bool isJumping;
    float jumpVelocity;
    glm::vec3 inputDir;

    Model model;
    Model gunModel;

    Animation idleAnim;
    Animation runAnim;
    
    Animation jumpstartAnim;
    Animation jumploopAnim;
	Animation jumpdownAnim;

	Animation shootAnim;

    Animator animator;

    // --- Blend system ---
    enum AnimState {
        IDLE,
        IDLE_RUN,
        RUN,
        RUN_IDLE,
        JUMP_START,
        JUMP_MID,
        JUMP_LAND,
        SHOOT
    };
    AnimState state = IDLE;

    float blendAmount = 0.0f;
    float blendRate = 0.1f;

    float jumpStartTimer = 0.0f;

    float shootCooldown = 0.25f;   // seconds between shots
    float shootTimer = 0.0f;       // countdown
    bool isShooting = false;       // for blending (optional)
    // --- Shooting / fire rate ---
    float fireRate = 0.25f;  // seconds per shot
    float fireTimer = 0.0f;  // counts down
    bool shootPressed = false;  // input tracking
    bool wasShootPressed = false;

    // --- Optional: for blending ---
    float shootBlendAmount = 0.0f;
    float shootBlendRate = 0.2f; // controls blending speed


    Player(const std::string& modelPath,
        const std::string& idlePath,
        const std::string& runPath,
		const std::string& jumpstartPath,
        const std::string& jumploopPath,
		const std::string& jumpdownPath,
		const std::string& shootPath,
        const std::string& gunPath,
        glm::vec3 startPos)
        : model(modelPath),
        idleAnim(idlePath, &model),
        runAnim(runPath, &model),
		jumpstartAnim(jumploopPath, &model),
        jumploopAnim(jumpstartPath, &model),
		jumpdownAnim(jumpdownPath, &model),
		shootAnim(shootPath, &model),
        animator(&idleAnim),
        gunModel(gunPath)
    {
        position = startPos;
        size = glm::vec3(0.5f, 1.0f, 0.5f);
        yaw = 180.0f;
		velocity = glm::vec3(0.0f);

        isMoving = false;
        isJumping = false;
        jumpVelocity = 0.0f;
        inputDir = glm::vec3 (0.0f);
    }

    void Update(float dt, bool up, bool down, bool left, bool right, bool jump, bool shoot)
    {

        //------------------------------------------------------------
        // Movement input
        //------------------------------------------------------------
        inputDir = glm::vec3(0.0f);
        if (up) inputDir.z -= 1.0f;
        if (down) inputDir.z += 1.0f;
        if (left) inputDir.x -= 1.0f;
        if (right) inputDir.x += 1.0f;

        bool wantsToMove = (glm::length(inputDir) > 0.1f);
        if (wantsToMove)
            inputDir = glm::normalize(inputDir);

        //------------------------------------------------------------
        // Acceleration / deceleration
        //------------------------------------------------------------
        velocity = glm::vec3(0.0f);

        const float accel = 100.0f;
        const float decel = 14.0f;

        if (wantsToMove)
            velocity += inputDir * accel * dt;
        else
        {
            float vLen = glm::length(velocity);
            if (vLen > 0.01f)
            {
                glm::vec3 vDir = velocity / vLen;
                float drop = decel * dt;
                velocity -= vDir * drop;
                if (glm::length(velocity) < drop)
                    velocity = glm::vec3(0.0f);
            }
        }

        isMoving = glm::length(velocity) > 0.05f;
        position += velocity * dt;

        if (isMoving)
        {
            float targetYaw = glm::degrees(atan2(velocity.x, velocity.z));
            float diff = fmodf(targetYaw - yaw + 540.0f, 360.0f) - 180.0f;
            yaw += diff * 10.0f * dt; // smooth turning
        }

        //------------------------------------------------------------
        // Jump physics
        //------------------------------------------------------------
        const float jumpForce = 8.5f;
        const float gravity = -20.0f;

        if (jump && !isJumping && position.y <= 0.01f)
        {
            isJumping = true;
            jumpVelocity = jumpForce;
            state = JUMP_START;
            blendAmount = 0.0f;
            jumpStartTimer = 0.0f;

            Animation* from = isMoving ? &runAnim : &idleAnim;
            animator.PlayAnimation(from, &jumpstartAnim, animator.m_CurrentTime, 0.0f, 0.0f);
        }

        if (isJumping)
        {
            position.y += jumpVelocity * dt;
            jumpVelocity += gravity * dt;

            if (position.y <= 0.0f)
            {
                position.y = 0.0f;
                isJumping = false;
                state = JUMP_LAND;
                blendAmount = 0.0f;

                Animation* endAnim = isMoving ? &runAnim : &idleAnim;
                animator.PlayAnimation(&jumpdownAnim, endAnim, animator.m_CurrentTime, 0.0f, 0.0f);
            }
        }

        //------------------------------------------------------------
        // Shooting system
        //------------------------------------------------------------
        if (fireTimer > 0.0f)
            fireTimer -= dt;

        bool shootPressedEdge = (shoot && !wasShootPressed);
        wasShootPressed = shoot;
        // shooting trigger only when the button is pressed THIS frame
        if (shootPressedEdge && fireTimer <= 0.0f && !isJumping && !isMoving)
        {
            fireTimer = fireRate;
            shootBlendAmount = 1.0f;

            Animation* baseAnim = isMoving ? &runAnim : &idleAnim;
            animator.PlayAnimation(baseAnim, &shootAnim, 0.0f, 0.0f, shootBlendAmount);

            animator.m_CurrentTime = 0.0f;  // <-- RESET TIME so we can detect duration
            state = SHOOT;
        }



        //------------------------------------------------------------
        // Animation blending
        //------------------------------------------------------------
        auto EaseInOut = [](float x) { return x * x * (3.0f - 2.0f * x); };

        switch (state)
        {
        case IDLE:
            if (!isJumping && isMoving)
            {
                blendAmount = 0.0f;
                state = IDLE_RUN;
                animator.PlayAnimation(&idleAnim, &runAnim, animator.m_CurrentTime, 0.0f, 0.0f);
            }
            break;

        case IDLE_RUN:
        {
            blendAmount += blendRate;
            float eased = EaseInOut(glm::clamp(blendAmount, 0.0f, 1.0f));
            animator.PlayAnimation(&idleAnim, &runAnim, animator.m_CurrentTime, animator.m_CurrentTime2, eased);

            if (blendAmount >= 1.0f)
            {
                animator.PlayAnimation(&runAnim, nullptr, animator.m_CurrentTime2, 0.0f, 0.0f);
                state = RUN;
                blendAmount = 0.0f;
            }
        }
        break;

        case RUN:
            if (!isMoving) { state = RUN_IDLE; blendAmount = 0.0f; }
            animator.PlayAnimation(&runAnim, nullptr, animator.m_CurrentTime, animator.m_CurrentTime2, 0.0f);
            break;

        case RUN_IDLE:
        {
            blendAmount += blendRate;
            float eased = EaseInOut(glm::clamp(blendAmount, 0.0f, 1.0f));
            animator.PlayAnimation(&runAnim, &idleAnim, animator.m_CurrentTime, animator.m_CurrentTime2, eased);

            if (blendAmount >= 1.0f)
            {
                animator.PlayAnimation(&idleAnim, nullptr, animator.m_CurrentTime2, 0.0f, 0.0f);
                state = IDLE;
                blendAmount = 0.0f;
            }
        }
        break;

        case JUMP_START:
        {
            jumpStartTimer += dt;
            float jumpStartDuration = jumpstartAnim.GetDuration() / jumpstartAnim.GetTicksPerSecond();
            animator.PlayAnimation(&jumpstartAnim, nullptr, animator.m_CurrentTime, 0.0f, 0.0f);

            if (jumpStartTimer >= jumpStartDuration)
            {
                state = JUMP_MID;
                animator.PlayAnimation(&jumploopAnim, nullptr, 0.0f, 0.0f, 0.0f);
                blendAmount = 0.0f;
            }
        }
        break;

        case JUMP_MID:
            if (jumpVelocity < -4.0f)
            {
                state = JUMP_LAND;
                blendAmount = 0.0f;
                animator.PlayAnimation(&jumploopAnim, &jumpdownAnim, animator.m_CurrentTime, 0.0f, 0.0f);
            }
            else
            {
                animator.PlayAnimation(&jumploopAnim, nullptr, animator.m_CurrentTime, 0.0f, 0.0f);
            }
            break;

        case JUMP_LAND:
        {
            blendAmount += blendRate * 1.4f;
            float eased = EaseInOut(glm::clamp(blendAmount, 0.0f, 1.0f));

            Animation* endAnim = isMoving ? &runAnim : &idleAnim;
            animator.PlayAnimation(&jumpdownAnim, endAnim, animator.m_CurrentTime, animator.m_CurrentTime2, eased);

            if (blendAmount >= 1.0f)
            {
                animator.PlayAnimation(endAnim, nullptr, animator.m_CurrentTime2, 0.0f, 0.0f);
                state = isMoving ? RUN : IDLE;
                blendAmount = 0.0f;
            }
        }
        break;

        case SHOOT:
        {
            float duration = shootAnim.GetDuration() / shootAnim.GetTicksPerSecond();

            // DO NOT restart shootAnim here!
            // ONLY update animator normally

            if (!shoot || animator.m_CurrentTime >= duration)
            {
                state = isMoving ? RUN : IDLE;
                shootBlendAmount = 0.0f;

                Animation* baseAnim = isMoving ? &runAnim : &idleAnim;
                animator.PlayAnimation(baseAnim, nullptr, animator.m_CurrentTime, 0.0f, 0.0f);
            }
        }
        break;
        }

        animator.UpdateAnimation(dt);
    }





    // ----------------------------------------------------
    // DRAW FUNCTION (unchanged)
    // ----------------------------------------------------
    void Draw(Shader& animShader, Shader& lightingShader,
        const glm::mat4& view, const glm::mat4& projection)
    {
        //------------------------------------------------------------
        // 1. Draw the animated player model
        //------------------------------------------------------------
        animShader.use();
        animShader.setMat4("projection", projection);
        animShader.setMat4("view", view);

        // upload bone matrices
        auto transforms = animator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            animShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

        // player world matrix
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, position);
        modelMat = glm::rotate(modelMat, glm::radians(yaw), glm::vec3(0, 1, 0));
        modelMat = glm::scale(modelMat, glm::vec3(0.5f));   // your character scale
        animShader.setMat4("model", modelMat);

        model.Draw(animShader);

        //------------------------------------------------------------
        // 2. Draw the gun, using the hand bone transform
        //------------------------------------------------------------
        const std::string handName = "mixamorig_RightHand";

        if (animator.m_BoneWorldTransforms.find(handName) == animator.m_BoneWorldTransforms.end())
        {
            std::cout << "ERROR: Hand bone NOT FOUND: " << handName << std::endl;
            return;
        }

        // Bone transform ALREADY in local model space (no modelMat!)
        glm::mat4 handTransform = animator.m_BoneWorldTransforms[handName];

        // small alignment offset (start small)
        glm::mat4 gunOffset = glm::mat4(1.0f);
        //gunOffset = glm::translate(gunOffset, glm::vec3(0.05f, -0.05f, 0.15f));
        gunOffset = glm::rotate(gunOffset, glm::radians(-90.0f), glm::vec3(0, 1, 0));
        gunOffset = glm::rotate(gunOffset, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        //gunOffset = glm::scale(gunOffset, glm::vec3(0.5f));  // adjust to your gun size

        // FINAL world transform:
        //      Player transform * Bone transform * alignment
        glm::mat4 gunMatrix = modelMat * handTransform * gunOffset;

        //------------------------------------------------------------
        // 3. Draw gun with lighting shader
        //------------------------------------------------------------
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", gunMatrix);

        gunModel.Draw(lightingShader);
    }

};


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
float obstacleSpacing = 20.0f;   // distance between obstacles
float laneOffset = 2.5f;         // X distance for lanes
int numObstacles = 10;           // pool size

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
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


    // ====== Load Players ======
    Player player1(
        FileSystem::getPath("resources/objects/Player/Rifle Aiming Idle.dae"),
        FileSystem::getPath("resources/objects/Player/Rifle Aiming Idle.dae"),
        FileSystem::getPath("resources/objects/Player/Rifle Run.dae"),
        FileSystem::getPath("resources/objects/Player/Jump Up.dae"),
        FileSystem::getPath("resources/objects/Player/Jump Loop.dae"),
        FileSystem::getPath("resources/objects/Player/Jump Down.dae"),
        FileSystem::getPath("resources/objects/Player/Firing Rifle.dae"),
        FileSystem::getPath("resources/objects/Player/model/gun/heavy_rifle.obj"),
        glm::vec3(5.0f, 0.0f, -5.0f)
    );

    Player player2(
        FileSystem::getPath("resources/objects/Player/Rifle Aiming Idle.dae"),
        FileSystem::getPath("resources/objects/Player/Rifle Aiming Idle.dae"),
        FileSystem::getPath("resources/objects/Player/Rifle Run.dae"),
        FileSystem::getPath("resources/objects/Player/Jump Up.dae"),
        FileSystem::getPath("resources/objects/Player/Jump Loop.dae"),
        FileSystem::getPath("resources/objects/Player/Jump Down.dae"),
        FileSystem::getPath("resources/objects/Player/Firing Rifle.dae"),
        FileSystem::getPath("resources/objects/Player/model/gun/heavy_rifle.obj"),
        glm::vec3(-5.0f, 0.0f, -5.0f)
    );

    //example animation
   /* Model ourModel(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"));
    Animation danceAnimation(FileSystem::getPath("resources/objects/vampire/dancing_vampire.dae"), &ourModel);
    Animator animator(&danceAnimation);*/

    std::vector<Obstacle> obstacles;
    for (int i = 0; i < numObstacles; i++)
    {
        float z = -i * obstacleSpacing - 10.0f;

        int lane = (rand() % 3) - 1;
        float x = lane * laneOffset;

        // smaller obstacle size (half width, lower height)
        glm::vec3 obsSize(0.5f, 0.5f, 0.5f);
        glm::vec3 obsPos(x, obsSize.y * 0.5f, z); // place on ground

        obstacles.push_back({ obsPos, obsSize });
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
        
        for (auto& obs : obstacles)
        {
            obs.position.z += worldSpeed * deltaTime;

            // Recycle obstacle when it passes the player
            if (obs.position.z > 5.0f)
            {
                obs.position.z = spawnZ;
                obs.position.x = ((rand() % 3) - 1) * laneOffset; // random lane
            }
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obs.position);
            model = glm::scale(model, obs.size /** 0.5f*/); // match collider dimensions
            lightingShader.setMat4("model", model);

            // You can use a cube VAO — if you don't have one, create it similarly to your plane VAO.
            renderCube(); // helper that draws a cube
        }

        //// 1. Camera offset in front of player (local space)
        //glm::vec3 cameraOffset(0.0f, 7.0f, -10.0f);

        //// 2. Rotate offset by player yaw (Y-axis only)
        //glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
        //    glm::radians(player1.yaw),
        //    glm::vec3(0, 1, 0));

        //glm::vec3 rotatedOffset = glm::vec3(rot * glm::vec4(cameraOffset, 1.0f));

        //// 3. Position camera
        //camera.Position = player1.position + rotatedOffset;

        //// 4. ALWAYS update Front, Right, Up properly
        //camera.Front = glm::normalize(player1.position - camera.Position);
        //camera.Right = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
        //camera.Up = glm::normalize(glm::cross(camera.Right, camera.Front));



        // Player 1 - WASD
        player1.Update(deltaTime,
            glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS);
        // Player 2 - Arrows
        player2.Update(deltaTime,
            glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

        // === COLLISION CHECKS ===
        for (auto& obs : obstacles)
        {
            // Player 1 collision
            if (CheckAABBCollision(player1.position, player1.size, obs.position, obs.size))
            {
                std::cout << "💥 Player 1 hit obstacle!" << std::endl;
                glfwSetWindowShouldClose(window, true);
                break;
            }

            // Player 2 collision
            if (CheckAABBCollision(player2.position, player2.size, obs.position, obs.size))
            {
                std::cout << "💥 Player 2 hit obstacle!" << std::endl;
                glfwSetWindowShouldClose(window, true);
                break;
            }
        }

        player1.Draw(animShader, lightingShader, view, projection);
        player2.Draw(animShader, lightingShader, view, projection);

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