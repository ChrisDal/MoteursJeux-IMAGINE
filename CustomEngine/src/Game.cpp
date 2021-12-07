#include "Game.h"


// GL 3.0 + GLSL 130
const char* glsl_version = "#version 130";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
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
}


Game::Game()
: m_shaderdir(PROJECT_DIR"src\\shaders\\"), 
  m_datadir(PROJECT_DIR"data\\"), 
  m_texturedir(PROJECT_DIR"data\\")
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
    VAO->addBuffer(VBO, layout);
     

    // 0. copy our vertices array in a buffer for OpenGL to use
    //glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    // 1. copy our index array in a element buffer for OpenGL to use
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0); 
    

    // Vertex & Fragment Shader
    std::string vertexsource = m_shaderdir; 
    vertexsource += "basicVertexShader.shader";

    std::string fragmentsource = m_shaderdir; 
    fragmentsource += "basicFragmentShader.shader";

    std::cout << fragmentsource << std::endl; 

    Shader* vertexShader = new Shader(vertexsource.c_str(), GL_VERTEX_SHADER);
    Shader* fragmentShader = new Shader(fragmentsource.c_str(), GL_FRAGMENT_SHADER);

    vertexShader->checkValidity(); 
    fragmentShader->checkValidity();

    // create shader Programe 
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

}

Game::~Game()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    delete VAO;
    delete VBO; 
    delete EBO; 
    
    delete shaderProgram; 
    delete boxTexture;
    
    
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
        
        static float f = 0.0f;
        static int counter = 0;
        static bool wireframeMode = false; 
        

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ====================================================
        // Draw here 
        //
        // 2. use our shader program when we want to render an object
        shaderProgram->setUniform1i("texture1", 0);
        shaderProgram->setUniform1i("texture2", 1);

        shaderProgram->use();
        
        // use color 
        //int location = shaderProgram->getUniformLocation("u_Color");
        //glUniform4f(location, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        
        // bind texture 
        boxTexture->bind(0); 
        faceTexture->bind(1); 
        
        // Bind our Vertex Array Object 
        VAO->bind();
        EBO->bind(); 
       

        // 6 = number of indices, unsigned int indices 
        glDrawElements(GL_TRIANGLES, EBO->getCount(), GL_UNSIGNED_INT, nullptr);
        VAO->unbind();
        EBO->unbind(); 
        boxTexture->unbind();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_Window);
        // glfwPoll
    }
}





