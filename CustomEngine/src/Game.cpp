#include "Game.h"
#include "Engine/Transform.h"
#include "Engine/GameObject.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// GL 3.0 + GLSL 130
const char* glsl_version = "#version 130";

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width; 
    SCR_HEIGHT = height; 
}


Game::Game()
: m_shaderdir(PROJECT_DIR"src\\shaders\\"), 
  m_datadir(PROJECT_DIR"data\\"), 
  m_texturedir(PROJECT_DIR"data\\"), 
  m_renderer((float)SCR_WIDTH, (float)SCR_HEIGHT)
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

    // enable Depth 
    glEnable(GL_DEPTH_TEST);


    // --------------------------
    /*float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };*/

    float vertices2[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };

    int nVertex = 9; 

    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    }; 

	/* TO REMOVE 
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), 
    // and then configure vertex attributes(s).
    VAO = new VertexArrayBuffer(); 
    VAO->bind(); 


    // Vertex Buffer 
    //VBO = new VertexBuffer(vertices, sizeof(vertices));
    VBO = new VertexBuffer(vertices2, sizeof(vertices2));

    // Index Buffer 
    unsigned int countIndices = sizeof(indices) / sizeof(unsigned int); 
    EBO = new IndexBuffer(indices, countIndices);

    // Set Layout 
    VertexBufferLayout layout = VertexBufferLayout();
    layout.Push<float>(3); // layout position 
    layout.Push<float>(3); // layout color
    layout.Push<float>(2); // layout texture
    
    // Add Layout to VAO 
    VAO->addBuffer(VBO, &layout);
     
    // Vertex & Fragment Shader
    std::string vertexsource = m_shaderdir; 
    vertexsource += "basicVertexShader.shader";

    std::string fragmentsource = m_shaderdir; 
    fragmentsource += "basicFragmentShader.shader";

    // Renderer Initialisation 
    // -------------------------

    // Shader Program : by Default 
    m_renderer.createShaderProg(vertexsource, fragmentsource);

    std::cout << fragmentsource << std::endl; 

    Shader* vertexShader = new Shader(vertexsource.c_str(), GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(fragmentsource.c_str(), GL_FRAGMENT_SHADER);

    vertexShader->checkValidity(); 
    fragmentShader->checkValidity();

    // create shader Program 
    shaderProgram = new ShaderProgram(); 
    shaderProgram->bindShaders(vertexShader, fragmentShader); 
    shaderProgram->link(); 
    shaderProgram->use(); 


    delete vertexShader; 
    delete fragmentShader; 

    // Unbind all 
    VAO->unbind(); 
    EBO->unbind();
    VBO->unbind(); 
    shaderProgram->unuse();


    // texture load 
    std::string imagecontainer = m_texturedir;
    imagecontainer += "container.jpg";
    boxTexture = new Texture(imagecontainer.c_str()); 

    std::string imageface = m_texturedir + std::string("awesomeface.png"); 
    faceTexture = std::make_unique<Texture>(imageface.c_str(), 1); 
	
	*/
	
	// Renderer 
	// ---------
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
	
	
	
	

    // Game Object  
    // -------------
    m_scene = new SceneNode();
    m_gmo = new GameObject(m_scene, glm::vec3(0.0, 0.0, 0.0), -1,"", "Player");
    m_gmo->initMesh(0);
                        

}

Game::~Game()
{
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

void Game::RunGameLoop()
{
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(m_Window))
    {

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

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        
        static glm::vec3 frotate = glm::vec3(0.0f, 0.0f, 0.0f); 
        static glm::vec3 ftranslate = glm::vec3(0.0f, 0.0f, 0.0f); 
        static int counter = 0;
        static bool wireframeMode = false; 
        

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("rotationX", &frotate.x, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("rotationY", &frotate.y, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("rotationZ", &frotate.z, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("translationX", &ftranslate.x, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("translationY", &ftranslate.y, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("translationZ", &ftranslate.z, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Wireframe Mode"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            wireframeMode = !wireframeMode;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        // input
        // -----
        processInput(m_Window);
        if (wireframeMode){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
        }

        // IMGUI rendering
        // -----------------
        ImGui::Render();

        // Render : Game LOOP 
        // ------------------

        int display_w, display_h;
        glfwGetFramebufferSize(m_Window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        m_renderer.Clear(); 

        //=================================================
        // GLM Transformations
        SpaceEngine::Transform transformation; 
        transformation.addRotation(frotate.x, frotate.y, frotate.z);
        //transformation.Translate(ftranslate.x, ftranslate.y, ftranslate.z);

        SpaceEngine::Transform transfo_extern;
        transfo_extern.Translate(0.5f, 0.0, 0.0);
        transfo_extern.addRotation(0.0f, frotate.y, 0.0f);
        //transformation.addHomogenousScale(0.5f); 

        SpaceEngine::Transform worldMatrix;
        worldMatrix.setAsIdentity();

        glm::vec4 vec(0.5f, 0.0f, 0.0f, 1.0f);
        //glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
        //trans = glm::rotate(trans, glm::radians(f), glm::vec3(0.0, 0.0, 1.0)); 
        //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        
        vec = transformation.getMatrixTransform() * vec;
		
		/*
		
        //-----------------
        // View Matrix 
        //-----------------
        // note that we're translating the scene in the reverse direction of where we want to move
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        //-----------------
        // Projection Matrix 
        //-----------------
        glm::mat4 projection;
        float ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT; 
        projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);


        // MVP Matrix 
        glm::mat4 mvp = projection * view; */
        
        //=================================================

        
        
        // =================================================
        // Draw here 
       
        // 2. use our shader program when we want to render an object
        /*shaderProgram->setUniform1i("texture1", 0);
        shaderProgram->setUniform1i("texture2", 1);*/
        
        // Transformations 
        /*shaderProgram->setMat4("u_mvp", glm::value_ptr(mvp));
        shaderProgram->setMat4("u_internal_tsfm_matrix", glm::value_ptr(transformation.getMatrixTransform()));
        shaderProgram->setMat4("u_worldmatrix", glm::value_ptr(worldMatrix.getMatrixTransform()));
        shaderProgram->setMat4("u_transform_matrix", glm::value_ptr(transfo_extern.getMatrixTransform()));*/
        
        // Exemple with BOX 
        // bind texture 
        /*boxTexture->bind(0);
        faceTexture->bind(1);
        m_renderer.Draw(VAO, EBO, shaderProgram); 
        boxTexture->unbind();*/


        // To Transform into 
        /*Mesh testmesh = Mesh(); 
        testmesh.initCube(); 
        m_renderer.Draw(&testmesh, -1);*/


        // translation of camera 
        m_renderer.setviewprojMat((float)SCR_WIDTH, (float)SCR_HEIGHT, ftranslate, true); 
        // Transformation = rotation 
        m_gmo->setTransformation(transformation); 
        m_renderer.Draw(m_gmo, GL_TRIANGLE_STRIP, -1); 

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_Window);
        // glfwPoll
    }
}







