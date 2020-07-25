#include "imgui_utils.h"
#include <imgui_internal.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/gl3w.h>

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLM Mathemtics
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"

#include "ExampleFBO.hpp"

namespace ImGui
{
    static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
    static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
    static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

#pragma region "test draw something in cpp with ImGui"
    GLuint framebuffer;
    GLuint cubeVAO, cubeVBO;
    GLuint floorVAO, floorVBO;
    GLuint cubeTexture;
    GLuint floorTexture;
    GLuint textureColorbuffer = 0;

    // Generates a texture that is suited for attachments to a framebuffer
    GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, int viewportWidth, int viewportHeight)
    {
        // What enum to use?
        GLenum attachment_type;
        if (!depth && !stencil)
            attachment_type = GL_RGB;
        else if (depth && !stencil)
            attachment_type = GL_DEPTH_COMPONENT;
        else if (!depth && stencil)
            attachment_type = GL_STENCIL_INDEX;

        //Generate texture ID and load texture data 
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        if (!depth && !stencil)
            glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, viewportWidth, viewportHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
        else // Using both a stencil and depth test, needs special format arguments
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewportWidth, viewportHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }

    void TestInitDrawInCPP(int viewportWidth, int viewportHeight)
    {
        // Define the viewport dimensions
        glViewport(0, 0, viewportWidth, viewportHeight);

        // Setup some OpenGL options
        glDepthFunc(GL_LESS);

#pragma region "object_initialization"
        // Set the object data (buffers, vertex attributes)
        GLfloat cubeVertices[] = {
            // Positions          // Texture Coords
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
        GLfloat floorVertices[] = {
            // Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
            5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

            5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
        };
        GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // Positions   // TexCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        // Setup cube VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glBindVertexArray(0);
        // Setup plane VAO
        glGenVertexArrays(1, &floorVAO);
        glGenBuffers(1, &floorVBO);
        glBindVertexArray(floorVAO);
        glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glBindVertexArray(0);

        // Load textures
        int width = 0; int height = 0;
        LoadTexture("container.jpg", &cubeTexture, &width, &height);
        LoadTexture("test_texture2.png", &floorTexture, &width, &height);

#pragma endregion

        // Framebuffers
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // Create a color attachment texture
        textureColorbuffer = generateAttachmentTexture(false, false, viewportWidth, viewportHeight);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        GLuint rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportWidth, viewportHeight); // Use a single renderbuffer object for both a depth AND stencil buffer.
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
        // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            glfw_error_callback(GL_FRAMEBUFFER_COMPLETE, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ImTextureID GetTestRenderInCPPFBO()
    {
        return (ImTextureID)textureColorbuffer;
    }

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    GLfloat fov = 45.0f;

    void TestRenderInCPP(int WIDTH, int HEIGHT)
    {
        /////////////////////////////////////////////////////
        // Bind to framebuffer and draw to color texture 
        // as we normally would.
        // //////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // Clear all attached buffers        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer so why bother with clearing?

        glEnable(GL_DEPTH_TEST);

        Shader shader("advanced.vs", "advanced.frag");
        // Set uniforms
        shader.Use();
        glm::mat4 model;
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // Projection 
        glm::mat4 projection;
        projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Floor
        glBindVertexArray(floorVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        model = glm::mat4();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // Cubes
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        /////////////////////////////////////////////////////
        // Bind to default framebuffer again and draw the 
        // quad plane with attched screen texture.
        // //////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
    }

    void TestFiniDrawInCPP()
    {
        glDeleteFramebuffers(1, &framebuffer);
    }
#pragma endregion
    GLFWwindow* gWindow;
//    int App_MainLoop(const char* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)())
//    {
//        // Setup window
//        glfwSetErrorCallback(glfw_error_callback);
//        if (!glfwInit())
//            return 1;
//
//        // Decide GL+GLSL versions
//#if __APPLE__
//    // GL 3.2 + GLSL 150
//        const char* glsl_version = "#version 150";
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
//#else
//    // GL 3.0 + GLSL 130
//        const char* glsl_version = "#version 130";
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
//#endif
//
// // Create window with graphics context
//        GLFWwindow* window = glfwCreateWindow(1280, 720, title, NULL, NULL);
//        if (window == NULL)
//            return 1;
//        gWindow = window;
//        glfwMakeContextCurrent(window);
//        glfwSwapInterval(1); // Enable vsync
//
//        // Initialize OpenGL loader
//#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
//        bool err = gl3wInit() != 0;
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
//        bool err = glewInit() != GLEW_OK;
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
//        bool err = gladLoadGL() == 0;
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
//        bool err = false;
//        glbinding::Binding::initialize();
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
//        bool err = false;
//        glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
//#else
//        bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
//#endif
//        if (err)
//        {
//            fprintf(stderr, "Failed to initialize OpenGL loader!\n");
//            return 1;
//        }
//
//
//        // Setup Dear ImGui context
//        IMGUI_CHECKVERSION();
//        ImGui::CreateContext();
//        ImGuiIO& io = ImGui::GetIO(); (void)io;
//        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
//        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
//        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
//        //io.ConfigViewportsNoAutoMerge = true;
//        //io.ConfigViewportsNoTaskBarIcon = true;
//
//        // Setup Dear ImGui style
//        ImGui::StyleColorsDark();
//        //ImGui::StyleColorsClassic();
//
//        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
//        ImGuiStyle& style = ImGui::GetStyle();
//        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//        {
//            style.WindowRounding = 0.0f;
//            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//        }
//
//        // Setup Platform/Renderer bindings
//        ImGui_ImplGlfw_InitForOpenGL(window, true);
//        ImGui_ImplOpenGL3_Init(glsl_version);
//
//        // Load Fonts
//        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//        // - Read 'docs/FONTS.md' for more instructions and details.
//        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//        //io.Fonts->AddFontDefault();
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
//        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//        //IM_ASSERT(font != NULL);
//
//        // Our state
//        bool show_demo_window = true;
//        bool show_another_window = false;
//        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//
//        OnInit();
//
//        // Main loop
//        while (!glfwWindowShouldClose(window))
//        {
//            // Poll and handle events (inputs, window resize, etc.)
//            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
//            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
//            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
//            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
//            glfwPollEvents();
//
//            // Start the Dear ImGui frame
//            ImGui_ImplOpenGL3_NewFrame();
//            ImGui_ImplGlfw_NewFrame();
//            ImGui::NewFrame();
//
//            OnGUI();
//
//            // Rendering
//            ImGui::Render();
//            int display_w, display_h;
//            glfwGetFramebufferSize(window, &display_w, &display_h);
//            glViewport(0, 0, display_w, display_h);
//            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//            glClear(GL_COLOR_BUFFER_BIT);
//            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//            // Update and Render additional Platform Windows
//            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
//            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
//            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//            {
//                GLFWwindow* backup_current_context = glfwGetCurrentContext();
//                ImGui::UpdatePlatformWindows();
//                ImGui::RenderPlatformWindowsDefault();
//                glfwMakeContextCurrent(backup_current_context);
//            }
//
//            glfwSwapBuffers(window);
//        }
//
//        OnExit();
//
//        // Cleanup
//        ImGui_ImplOpenGL3_Shutdown();
//        ImGui_ImplGlfw_Shutdown();
//        ImGui::DestroyContext();
//
//        glfwDestroyWindow(window);
//        glfwTerminate();
//
//        return 0;
//    }
    
    struct ImGuiViewportDataGlfw
    {
        GLFWwindow* Window;
        bool        WindowOwned;
        int         IgnoreWindowPosEventFrame;
        int         IgnoreWindowSizeEventFrame;

        ImGuiViewportDataGlfw() { Window = NULL; WindowOwned = false; IgnoreWindowSizeEventFrame = IgnoreWindowPosEventFrame = -1; }
        ~ImGuiViewportDataGlfw() { IM_ASSERT(Window == NULL); }
    };

    void SetWindowTitle(ImGuiViewport* viewport, const char* title)
    {
        ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
        glfwSetWindowTitle(data->Window, title);
    }

    bool LoadTexture(const char* filename, unsigned int* texture_id, int* out_width, int* out_height)
    {
        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return false;

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *texture_id = image_texture;
        *out_width = image_width;
        *out_height = image_height;
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }


    bool ImageAnimButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void*)user_texture_id);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
        const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        // Render
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
        if (held)
        {
            window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv2, uv3, GetColorU32(tint_col));
        }
        else
        {
            window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
        }

        return pressed;
    }


    bool ToggleButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, bool* v, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void*)user_texture_id);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
        const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);
        if (ImGui::IsItemClicked())
            *v = !*v;

        // Render
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
        if (*v || held)
        {
            window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv2, uv3, GetColorU32(tint_col));
        }
        else
        {
            window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
        }

        return v;
    }

    void ToggleButton(const char* str_id, bool * v)
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        float height = ImGui::GetFrameHeight();
        float width = height * 1.55f;
        float radius = height * 0.50f;

        ImGui::InvisibleButton(str_id, ImVec2(width, height));
        if (ImGui::IsItemClicked())
            *v = !*v;

        float t = *v ? 1.0f : 0.0f;

        ImGuiContext& g = *GImGui;
        float ANIM_SPEED = 0.08f;
        if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
        {
            float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
            t = *v ? (t_anim) : (1.0f - t_anim);
        }

        ImU32 col_bg;
        if (ImGui::IsItemHovered())
            col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
        else
            col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
        draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
    }

    bool RadioButton(ImTextureID tid_active, ImTextureID tid_deactivate, const ImVec2& size, bool active)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void*)tid_active);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + size);
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            MarkItemEdited(id);

        RenderNavHighlight(total_bb, id);

        if (active)
        {
            window->DrawList->AddImage(tid_active, total_bb.Min, total_bb.Max, ImVec2(0, 0), ImVec2(1, 1));
        }
        else
        {
            window->DrawList->AddImage(tid_deactivate, total_bb.Min, total_bb.Max, ImVec2(0, 0), ImVec2(1, 1));
        }

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
        return pressed;
    }

    // FIXME: This would work nicely if it was a public template, e.g. 'template<T> RadioButton(const char* label, T* v, T v_button)', but I'm not sure how we would expose it..
    bool RadioButton(ImTextureID tid_active, ImTextureID tid_deactivate, const ImVec2& size, int* v, int v_button)
    {
        const bool pressed = RadioButton(tid_active, tid_deactivate, size, *v == v_button);
        if (pressed)
            *v = v_button;
        return pressed;
    }

    static ExampleFBO s_fbo;
    void bgfxInit(int width, int height)
    {
        bgfx::PlatformData pd;
        bx::memSet(&pd, 0, sizeof(pd));
        pd.nwh = gWindow;
        bgfx::setPlatformData(pd);

        s_fbo.init(width, height);
        bgfx::frame();
    }

    void bgfxTick()
    {
        s_fbo.update();
    }

    void bgfxFini()
    {
        s_fbo.shutdown();
    }

    ImTextureID getbgfxfob()
    {
        //return (void*)(intptr_t)bgfx::getTexture(s_fbo.m_fbh).idx;
        //return (ImTextureID)bgfx::getTexture(s_fbo.m_fbh).idx;
        return (void*)(intptr_t)s_fbo.toImGuiTextureId(s_fbo.m_texture);
    }


}