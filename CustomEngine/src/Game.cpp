#include "Game.h"
#include "Engine/Transform.h"
#include "Engine/GameObject.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// GL 3.0 + GLSL 130
const char* glsl_version = "#version 130";

// settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static constexpr float APP_MAX_FRAMERATE{ 60.0f };
bool callbackWindows = false;
bool firstMouse = true; 
float lastX = SCR_WIDTH / 2.0f; 
float lastY = SCR_HEIGHT / 2.0f;
glm::vec2 Game::camDeadZone = glm::vec2((float)SCR_WIDTH/5.0f, (float)SCR_HEIGHT / 5.0f);
float Game::camoffsetx = 0.0f;
float Game::camoffsety = 0.0f;
bool processCamera = true; 
bool cameraRotation = true; 
bool useInternal = false; 



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width; 
    SCR_HEIGHT = height; 

    callbackWindows = true; 

    
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos); 

Game::Game()
: m_shaderdir(PROJECT_DIR"src\\shaders\\"), 
  m_datadir(PROJECT_DIR"data\\"), 
  m_texturedir(PROJECT_DIR"data\\"), 
  m_renderer((float)SCR_WIDTH, (float)SCR_HEIGHT), 
  m_lastTime(std::chrono::steady_clock::now()), 
  m_startTime(std::chrono::steady_clock::now())
{
    // GLFW and ImGui initialisation 
    initWindow(); 

    // Renderer initialisation 
    m_renderer.initGraphics(); 
    m_renderer.printOpenGLVersion(); 
    

    /*
    // texture load 
    std::string imagecontainer = m_texturedir;
    imagecontainer += "container.jpg";
    boxTexture = new Texture(imagecontainer.c_str()); 

    std::string imageface = m_texturedir + std::string("awesomeface.png"); 
    faceTexture = std::make_unique<Texture>(imageface.c_str(), 1); 
	
	*/
	
	// Default Shader 
	// ---------------
	// Vertex & Fragment Shader
    std::string vertexsource = m_shaderdir; 
    vertexsource += "basicVShader.shader";

    std::string fragmentsource = m_shaderdir; 
    fragmentsource += "basicFragShader.shader";

    // Renderer Initialisation 
    // -------------------------

    // Shader Program : by Default 
    m_renderer.createShaderProg(vertexsource, fragmentsource);
    std::cout << fragmentsource << std::endl; 

    this->initScene(); 
	



    // --------------------------------------------
    // Game Player 
    // -----------
    
    retrievePlayer(m_scene); 
    if (m_player == nullptr)
    {
        std::cout << "No player found.\n"; 
    }
    // --------------------------------------------

}

void Game::initWindow()
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

    // glfw window creation
    // --------------------
    m_Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SpaceEngine", NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
    // Mouse Inputs 
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPosCallback(m_Window, mouse_callback);

    



    // ImGUI 
    // Setup Dear ImGui context
    // ---------------------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // stbi image loading preset 
    stbi_set_flip_vertically_on_load(true);

}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cameraRotation) { 
        Game::camoffsetx = 0.0f;
        Game::camoffsety = 0.0f;
        return;  
    }
    
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    // In dead Zone wont treat
    glm::vec2 xDZN{ ((float)SCR_WIDTH / 2.0f) - Game::camDeadZone.x ,
                    ((float)SCR_WIDTH / 2.0f) + Game::camDeadZone.x };
    glm::vec2 yDZN{ ((float)SCR_HEIGHT / 2.0f) - Game::camDeadZone.y ,
                    ((float)SCR_HEIGHT / 2.0f) + Game::camDeadZone.y };

    bool inDeadZoneX = static_cast<float>(xDZN.x) < xpos && xpos < static_cast<float>(xDZN.y);
    bool inDeadZoneY = static_cast<float>(yDZN.x) < ypos && ypos < static_cast<float>(yDZN.y);

    if (inDeadZoneX && inDeadZoneY)
    {
        processCamera = false;
        return;
    }


    Game::camoffsetx = static_cast<float>(xpos) - lastX;
    Game::camoffsety = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    processCamera = true; 
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::processInput(GLFWwindow* window, bool internal)
{
    GameObject* actor = static_cast<GameObject*>(m_player);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movement += sensitivity * Game::m_zneg; 
        std::cout << "U-";
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movement += sensitivity * Game::m_zpos;
        std::cout << "B-";
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movement += sensitivity * Game::m_right;
        std::cout << "R-";
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movement += sensitivity * Game::m_left;
        std::cout << "L-";
    }

    if (movement.length() != 0.0)
    {
        actor->Translate(movement, useInternal);
    }

    // Mouse 
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        mouse_left_down = true;

    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        mouse_left_down = false;
    }

    /*if (mouse_left_down)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        setTrackingPoint(window, m_camera, xpos, ypos);
    }*/


    if (actor->velocity.ismoving()) {
        glm::vec4 pos = actor->getWorldPosition();
        std::cout << " Actor : " << pos.x << pos.y << pos.z << "\n";
        glm::vec3 targetActor{ pos.x, pos.y, pos.z };
        m_camera->setTargetPoint(targetActor);
        m_camera->setWalking(true);
    }
    else
    {
        m_camera->setWalking(false);
    }

   
}

Game::~Game()
{
    std::cout << "\nGame Destructor\n"; 
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    if (VAO != nullptr){ delete VAO; }
    if (VBO != nullptr){ delete VBO; }
    if (EBO != nullptr) { delete EBO; } 
    if (shaderProgram != nullptr) { delete shaderProgram; }
    //delete boxTexture;
    delete m_scene; 
    
    
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_Window);

    glfwTerminate();

}

// Find player through SceneGraph
void Game::retrievePlayer(SceneNode* root)
{
    if (root->haveGmo())
    {
        if (root->getObject()->isPlayer()) {
            m_player = root->getObject();
            return;
        }
    }
    

    for (int i = 0; i < root->getChildrenNumber(); i++)
    {
        retrievePlayer(root->getNode(i));
    }
}

void Game::RunGameLoop()
{
    double frameTime{ 1.0f / APP_MAX_FRAMERATE }; // in seconds 
    m_lastTime = std::chrono::steady_clock::now();
    double mFrameTime = 0.0; 
    bool render = false; 

    // GET nodes 
    SceneNode* sunNode = m_scene->getNodebyId("SN6");
    SceneNode* satPlayer = m_scene->getNodebyId("SN3"); 

    m_camera->processMovement(Game::camoffsetx, Game::camoffsety);

    
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(m_Window))
    {
        m_startTime = std::chrono::steady_clock::now();
        double deltaTime = 0.001 * std::chrono::duration_cast<std::chrono::milliseconds>(m_startTime - m_lastTime).count();
        
        m_lastTime = m_startTime;
        mFrameTime += deltaTime;

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        
        // input
        // -----
        processInput(m_Window, useInternal);
        

        /*if (processCamera && cameraRotation)
        {
            m_camera->processMovement(Game::camoffsetx, Game::camoffsety);
        }*/

        while (mFrameTime >= 1.0f/APP_MAX_FRAMERATE) 
        {
            // Update Game 
            // -----------
            // Test Graph Scene 
            sunNode->Rotate(0.0f, 1.0f, 0.0f, false);
            satPlayer->getObject()->Rotate(-0.5f, 0.0f, 0.0f, true);

            
            Update(static_cast<float>(deltaTime));

            mFrameTime -= 1.0f / APP_MAX_FRAMERATE;
            
        }
        
        // IMGUI rendering
        // -----------------
        RenderDebugMenu();
        ImGui::Render();

        // Render : Game LOOP 
        // ------------------

        int display_w, display_h;
        glfwGetFramebufferSize(m_Window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        if (callbackWindows)
        {
            m_camera->setPerspective(0.1f, 100.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT); 
            callbackWindows = false; 
        }
        
        
        
        //=================================================
        // Draw here 

        // Exemple with Texture
        // bind texture 
        /*boxTexture->bind(0);
        faceTexture->bind(1);
        m_renderer.Draw(VAO, EBO, shaderProgram);
        boxTexture->unbind();*/

        // render
        // ------
        
        m_renderer.Clear();
        // Transformation 
        m_renderer.Draw(m_scene);


        // =================================================

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_Window);
        // glfwPoll



    }
}





void Game::Update(float deltaTime)
{
    m_scene->Update(deltaTime); 
}

// Display a node Scene in ImGUI
static void displayGraphNode(SceneNode* node, int& selectable)
{
    // Node ID 
    if (ImGui::TreeNode(node->getId().c_str()))
    {
        // Display GameObject 
        if (node->haveGmo())
        {
            int ngmo = node->getObject()->getId(); 
            const std::string label = "GameObj " + std::to_string(ngmo);
            if (ImGui::Selectable(label.c_str(), selectable == ngmo)) {
                selectable = ngmo;
            }
            ImGui::Text("Tag:"); 
            ImGui::SameLine();
            ImGui::Text(node->getObject()->getTag().c_str());
            if (node->getObject()->hasMesh())
            {
                ImGui::SameLine();
                
                if (ImGui::SmallButton("Set Red")) {
                    GameObject* gmo = (GameObject*)node->getObject();
                    if (gmo->getMesh()->getColor() == Mesh::basicColor)
                    {
                        gmo->getMesh()->setColor(1.0f, 0.0f, 0.0f, 1.0f);
                    }
                    else
                    {
                        gmo->getMesh()->setColor(Mesh::basicColor);
                    }
                    
                }
            }
            
                
        }

        
        for (int i = 0; i < node->getChildrenNumber(); i++)
        {
            displayGraphNode(node->getNode(i), selectable);
        }

        
        ImGui::TreePop();    
    }

    
}; 

void Game::DisplayUISceneGraph(SceneNode* root, int& selected)
{
    // Scene Graph 
    if (ImGui::CollapsingHeader("SceneGraph"))
    {
        

        if (m_scene != nullptr)
        {
            // Nodes 
            ImGui::SetNextTreeNodeOpen(true);
            displayGraphNode(m_scene, selected); 
        }
    }

    
}

void Game::RenderDebugMenu() {

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.

    static glm::vec3 frotate = glm::vec3(0.0f, 0.0f, 0.0f);
    static glm::vec3 ftranslate = glm::vec3(0.0f, 0.0f, 0.0f);
    static int counter = 0;
    static bool wireframeMode = false;
    static bool orthoprojection = false;

    // Transforms
    static glm::mat3 objtransfm; 
    static glm::vec3 transVec3 = { 0.0f, 0.0f, 0.0f };
    static glm::vec3 rotVec3   = { 0.0f, 0.0f, 0.0f };
    static glm::vec3 scaleVec3 = { 1.0f, 1.0f, 1.0f };
    static const char* axesnames[3] = { "X", "Y", "Z" }; 
    static bool applyTransform[3] = { false, false, false }; // TRS Modifications have been made

    // ImGui TreeNode
    ImGui::Begin("Inspector");
    ImGui::SetNextTreeNodeOpen(true);

    // Graphe de Scene 
    static int selected = -1;
    BasicGameObject* foundObj = nullptr;
    DisplayUISceneGraph(m_scene, selected);

    if (selected != -1) {
        
        foundObj = m_scene->getObjectbyID(selected);

        if (foundObj != nullptr)
        {
            objtransfm = foundObj->getTransformation().getPackedTransform();
            rotVec3 = objtransfm[0];
            transVec3 = objtransfm[1];
            scaleVec3 = objtransfm[2]; 
        }
    }

        

    ImGui::SetNextTreeNodeOpen(true);
    
    // Transforms
    if (ImGui::CollapsingHeader("Transformations"))
    {
        // Translation
        {
            ImGui::Text("Translation"); 
            ImGui::PushItemWidth(80);
            for (int i = 0; i < 3; i++)
            {
                // X , Y , Z 
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
                std::string label = std::string(axesnames[i]) + "##" + std::string("Tsl");
                if (ImGui::Button(label.c_str())) {
                    // Reset If its clicked
                    transVec3[i] = 0.0f;
                    applyTransform[0] = true;
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Reset to 0.0f");
                }
            
                ImGui::PopStyleColor(3);
                ImGui::PopID();
                ImGui::SameLine(0.0f, 1.0f); 

                // Give Unique ID for Sliders but do not display
                std::string uid = "##" + std::string("Tsl") + std::string(axesnames[i]);
                applyTransform[0] |= ImGui::DragFloat(uid.c_str(), &transVec3[i], 0.1f, -FLT_MAX, FLT_MAX);
                if (ImGui::IsItemHovered()) {
                    std::string message = "Translation over axe " + std::string(axesnames[i]);
                    ImGui::SetTooltip(message.c_str());
                }

                if (i == 2) {
                    continue; 
                }

                ImGui::SameLine();
            }

            ImGui::PopItemWidth();

        }

        // Rotation
        {

            ImGui::Text("Rotation");
            ImGui::PushItemWidth(80);
            for (int i = 0; i < 3; i++)
            {
                // X , Y , Z 
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
                std::string label = std::string(axesnames[i]) + "##" + std::string("Rot") ;
                if (ImGui::Button(label.c_str())) {
                    // Reset If its clicked
                    rotVec3[i] = 0.0f;
                    applyTransform[1] = true;
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Reset to 0.0f");
                }

                ImGui::PopStyleColor(3);
                ImGui::PopID();
                ImGui::SameLine(0.0f, 1.0f);

                // Give Unique ID for Sliders but do not display
                std::string uid = "##" + std::string("Rot") + std::string(axesnames[i]);
                applyTransform[1] |= ImGui::DragFloat(uid.c_str(), &rotVec3[i], 1.0f, -359.99f, 360.0f);
                if (ImGui::IsItemHovered()) {
                    std::string message = "Rotation -359:360 over axe " + std::string(axesnames[i]);
                    ImGui::SetTooltip(message.c_str());
                }

                if (i == 2) {
                    continue;
                }

                ImGui::SameLine();
            }

            ImGui::PopItemWidth();

        }

        // Scale
        {
            ImGui::Text("Scale");
            ImGui::PushItemWidth(80);
            for (int i = 0; i < 3; i++)
            {
                // X , Y , Z 
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
                std::string label = std::string(axesnames[i]) + "##" + std::string("Scl");
                if (ImGui::Button(label.c_str())) {
                    // Reset If its clicked
                    scaleVec3[i] = 1.0f;
                    applyTransform[2] = true;
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Reset to 1.0f");
                }

                ImGui::PopStyleColor(3);
                ImGui::PopID();
                ImGui::SameLine(0.0f, 1.0f);

                // Give Unique ID for Sliders but do not display
                std::string uid = "##" + std::string("Scl") + std::string(axesnames[i]);
                applyTransform[2] |= ImGui::DragFloat(uid.c_str(), &scaleVec3[i], 0.01f, 0.0f, 100.0f);
                if (ImGui::IsItemHovered()) {
                    std::string message = "Scale 0:100 over axe " + std::string(axesnames[i]);
                    ImGui::SetTooltip(message.c_str());
                }

                if (i == 2) {
                    continue;
                }

                ImGui::SameLine();
            }

            ImGui::PopItemWidth();
        }

        ImGui::Separator();

        ImGui::Text("Color");
        ImGui::ColorEdit3("GameObject", (float*)&clear_color); // Edit 3 floats representing a color

    }

    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::CollapsingHeader("Divers"))
    {
        // Checkbox for scene
        ImGui::Checkbox("Wireframe Mode", &wireframeMode);
        ImGui::Checkbox("Internal Transform translation", &useInternal);
        
    }

    ImGui::End();


    // Apply Transformation for selected obj : TRS
    if (applyTransform[0] && foundObj != nullptr)
    {
        foundObj->setTranslate(transVec3, true);
    }

    if (applyTransform[1] && foundObj != nullptr)
    {
        foundObj->setRotate(rotVec3, true);
    }

    if (applyTransform[2] && foundObj != nullptr)
    {
        foundObj->setScale(scaleVec3, true);
    }
    
    // ImGui example menu overlay 
    static bool show_app_simple_overlay = true;
    const float DISTANCE = 10.0f;
    static int corner = 3;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", &show_app_simple_overlay, window_flags))
    {
        ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
        {
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        }
        else {
            ImGui::Text("Mouse Position: <invalid>");
        }


        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (&show_app_simple_overlay && ImGui::MenuItem("Close")) show_app_simple_overlay = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    m_renderer.setPolymode(!wireframeMode);

    if (m_camera != nullptr)
    {
        m_renderer.setviewprojMat(m_camera->getLookAt(), m_camera->getPerspective()); 
    }

}



void Game::handleInput(GLFWwindow* window, GameObject* actor, Camera* cam)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }



    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        actor->velocity.vz -= 5.0f;
        std::cout << "Up Actor - ";
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        actor->velocity.vz += 5.0f;
        std::cout << "BAck Actor - ";
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        actor->velocity.vx += 5.0f;
        std::cout << "Right Actor - ";
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        actor->velocity.vx -= 5.0f;

        std::cout << "Left Actor - ";
    }

    // Mouse 
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        mouse_left_down = true;

    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        mouse_left_down = false;
    }

    /*if (mouse_left_down)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        setTrackingPoint(window, cam, xpos, ypos);
    }*/


    if (actor->velocity.ismoving()) {
        glm::vec4 pos = actor->getWorldPosition();
        glm::vec3 targetActor{ pos.x, pos.y, pos.z };
        cam->setTargetPoint(targetActor);
        cam->setWalking(true);
    }
    else
    {
        cam->setWalking(false);
    }

}

void Game::setTrackingPoint(GLFWwindow* window, Camera* cam, double& x, double& y)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);

    // centered target
    glm::vec3 newTargetPoint = glm::vec3(x, y, 0.0f) - glm::vec3((float)w / 2.0f, (float)h / 2.0f, 0.0f);
    glm::vec3 oldTargetPoint = cam->getTargetPoint();
    // direction
    glm::vec3 direction = newTargetPoint - oldTargetPoint;
    direction.z = 0.0f;
    direction = glm::normalize(direction);

    newTargetPoint = oldTargetPoint + glm::vec3(direction.x * cam->velocity.vx,
        -direction.y * cam->velocity.vy,
        0.0f);

    cam->setTargetPoint(newTargetPoint);

}

void Game::initScene()
{
    // Scene Init  
    // -------------
    m_scene = new SceneNode(); //  ROOT NODE 

    // Player
    SceneNode* nodePlayer = new SceneNode(m_scene, glm::vec3(0.0f, 0.0f, 0.0f));
    GameObject* player = new GameObject(nodePlayer, glm::vec3(2.0, 0.0, 0.0), -1, "", "Player");
    player->initMesh(2);
    player->velocity.setVelocity(0.0f, 0.0f, 0.0f);

    SceneNode* nodeSatPlayer = new SceneNode(nodePlayer);
    GameObject* satPlayer = new GameObject(nodeSatPlayer, glm::vec3(0.0f, 1.0f, 0.0f), -1);
    satPlayer->initMesh(0); 
    satPlayer->Scale(0.1f, 0.1f, 0.1f, true); 

    // Camera Node 
    SceneNode* cameraNode = new SceneNode(nodePlayer, glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera = new Camera(cameraNode, glm::vec3(0.0, 0.0, 8.0));
    m_camera->setTargetPoint(glm::vec3(0.0f, 0.0f, 0.0f));
    m_camera->setPerspective(0.1f, 100.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);

    // System Solar 
    SceneNode* solarNode = new SceneNode(m_scene, glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Sun Node 
    SceneNode* suNode = new SceneNode(solarNode, glm::vec3(0.0f, 0.0f, 0.0f));
    GameObject* sun = new GameObject(suNode, glm::vec3(0.0f, 0.0, 0.0f), -1);
    sun->initMesh(3);
    
    // Mars Node 
    SceneNode* marsNode = new SceneNode(solarNode, glm::vec3(0.0f, 0.0f, 0.0f));
    GameObject* mars = new GameObject(marsNode, glm::vec3(3.0f, 0.0, 0.0f), -1);
    mars->initMesh(3);
    SpaceEngine::Transform transfolune;
    transfolune.setHomogenousScale(0.35f); 
    transfolune.setRotation(0.0f, 0.0f, 0.0f);
    mars->addTransformation(transfolune, true);

    // MarsSat  Node 
    SceneNode* marsSatNode = new SceneNode(marsNode);
    GameObject* marsSat = new GameObject(marsSatNode, glm::vec3(1.0f, 1.0f, 1.0f), -1);
    marsSat->initMesh(3);
    SpaceEngine::Transform transfoSat;
    transfoSat.setHomogenousScale(0.2f);
    transfoSat.setRotation(0.0f, 65.0f, 15.0f);
    marsSat->addTransformation(transfoSat, true);


    // Ajout Planete 
    {
        SceneNode* planeteNode = this->addPlanet(solarNode, glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.5f), 0.25f);
    }
    
    // Ajout Planete + Satellite 
    {
        SceneNode* planeteNode = this->addPlanet(solarNode, glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(4.0f, 0.0f, 2.0f), 0.6f,
            glm::vec3(0.0f, 23.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f));

        SceneNode* satelliteNode = this->addSatellite(planeteNode, glm::vec3(0.0f, 2.0f, 0.0f),
            0.2f, glm::vec3(0.0f, 15.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f));
    }

    // Venus Node 
    /*SceneNode* venusNode = new SceneNode(suNode, glm::vec3(0.0f, 0.0f, 0.0f));
    GameObject* venus = new GameObject(venusNode, glm::vec3(5.0f, 0.0, 0.0f), -1);
    venus->initMesh(3);
    SpaceEngine::Transform transfonus;
    transfonus.setHomogenousScale(0.6f);
    transfonus.setRotation(0.0f, 23.0f, 0.0f);
    venus->addTransformation(transfonus, true);

    // VenusSat  Node 
    SceneNode* venusSatNode = new SceneNode(venusNode);
    GameObject* venusSat = new GameObject(venusSatNode, glm::vec3(0.0f, 2.0f, 0.0f), -1);
    venusSat->initMesh(3);
    SpaceEngine::Transform transfovenusSat;
    transfovenusSat.setHomogenousScale(0.2f);
    transfovenusSat.setRotation(0.0f, 0.0f, 15.0f);
    venusSat->addTransformation(transfovenusSat, true);*/


    /*GameObject* other = new GameObject(etape1, glm::vec3(0.0, 0.0, 0.0), -1);
    std::string meshfilepath = m_datadir;
    meshfilepath += "\\models\\cube.obj";
    other->initMesh(meshfilepath.c_str());


    // Terre : Sphere 
    SpaceEngine::Transform transfoterre;
    transfoterre.addHomogenousScale(0.7f);
    //transfoterre.addRotation(0.0f, 23.0f, 0.0f);
    GameObject* terre = new GameObject(etape1, glm::vec3(3.0, 0.0, 0.0), -1);
    terre->initMesh(3);
    terre->addTransformation(transfoterre, true);

    GameObject* ground = new GameObject(m_scene, glm::vec3(0.0, -2.0, 0.0),
                                        -1, "", "Terrain");
    ground->initMesh(1); */



    // -------------------------------------------
    m_scene->print();
    m_scene->sceneInit(m_scene);
}



SceneNode* Game::addPlanet(SceneNode* root, const glm::vec3& nodepos,
                const glm::vec3& planetpos, float scale, const glm::vec3& rotation,
                const glm::vec3& translation)
{
    // Venus Node 
    SceneNode* planetNode = new SceneNode(root, nodepos);
    GameObject* planet = new GameObject(planetNode, planetpos, -1);
    planet->initMesh(3); // by default its round

    // Add an internal transformation 
    SpaceEngine::Transform transfoplanet(glm::vec3(scale, scale, scale), 
                                        Rotoform(rotation.x, rotation.y, rotation.z, false), 
                                        translation);
    planet->addTransformation(transfoplanet, true);

    return planetNode; 
}


SceneNode* Game::addSatellite(SceneNode* planetNode, const glm::vec3& satpos,
                float scale, const glm::vec3& rotationInDegree,
                const glm::vec3& translation)
{
    // Satellite  Node 
    SceneNode* satNode = new SceneNode(planetNode);
    GameObject* planetSat = new GameObject(satNode, satpos, -1);
    planetSat->initMesh(3);

    // Add an internal transformation 
    SpaceEngine::Transform transfoSat(glm::vec3(scale, scale, scale),
                                    Rotoform(rotationInDegree.x, rotationInDegree.y, rotationInDegree.z, false),
                                    translation);
    planetSat->addTransformation(transfoSat, true);

    return satNode; 
}

