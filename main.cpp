#include <application.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <shader.hpp>
#include <memory>
#include <utility>
#include <texture/texture-utils.h>
#include <glm/gtx/euler_angles.hpp>
#include <json/json.hpp>
#include <mesh/common-vertex-types.hpp>
#include <mesh/common-vertex-attributes.hpp>
#include "../ECS/Entity/Entity.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/CameraControllerComponent.h"
#include "../ECS/Component/MeshRenderer.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <vector>
#include <texture/Texture2D.h>
#include <texture/Sampler.h>

#include "Component/LightComponent.h"
#pragma region helper_functions

std::string read_file(const char *filename)
{
    std::ifstream fin(filename);
    if (fin.fail())
    {
        std::cerr << "Unable to open shader file: " << filename << std::endl;
        std::exit(-1);
    }
    return std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());
}
void checkShaderCompilationErrors(GLuint shader)
{
    //Check and log for any error in the compilation process.
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // Takes a shader and returns the status of this shader program.
    if (!status)
    { // If there is a status (status != 0):
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length); // Get the error length (char array length).
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr); // Get the error char array.
        std::cerr << "ERROR:" << logStr << std::endl;        // print the char array of the log error.
        delete[] logStr;
        std::exit(-1);
    }
}
void attachShader(GLuint program, const char *filename, GLenum shader_type)
{

    // 1. Reads the shader from file, compiles it,
    std::string source_code = read_file(filename);
    const char *source_code_as_c_str = source_code.c_str();

    // 2. Pass the program as a string to the GPU and then compile it.
    GLuint shader = glCreateShader(shader_type);
    // Function parameter:
    // shader (GLuint): shader object name.
    // count (GLsizei): number of strings passed in the third parameter. We only have one string here.
    // string (const GLchar**): an array of source code strings.
    // lengths (const GLint*): an array of string lengths for each string in the third parameter. if null is passed,
    //                          then the function will deduce the lengths automatically by searching for '\0'.
    glShaderSource(shader, 1, &source_code_as_c_str, nullptr);
    glCompileShader(shader);


    // 3. Check for any Compilation errors.
    checkShaderCompilationErrors(shader);

    // 4. Attach this shader to the program if no errors found in shader.
    glAttachShader(program, shader);

    // 5. Delete the shader as it is already attached to the program.
    glDeleteShader(shader);
}
// Reads a file using a path and returns the file text in a string.
void checkProgramLinkingErrors(GLuint program)
{
    //Check and log for any error in the linking process.
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status); // Takes a shader program (vertex & fragment) and returns the status of this shader program.
    if (!status)                                      // If there is a status (status != 0):
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length); // Get the error length (char array length).
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr); // Get the error char array.
        std::cerr << "LINKING ERROR: " << logStr << std::endl; // print the char array of the log error.
        delete[] logStr;
        std::exit(-1);
    }
}

#pragma endregion

GLuint program = 0, vertex_array = 0;
int keyShaderOption = 2;
glm::vec2 mouse = glm::vec2(0, 0);
GameState *switchState(int state);

class ShaderIntroductionApplication : public our::Application
{

    // These unsigned integers represent the way we communicate with the GPU.
    // They act like a name (or, in other words, an ID).
    // These uint are passed to the GL header functions to tell the GL which OpenGL object
    // we are referring to. Also the values are set by the GL functions when passed by reference.

    // We need a window with title: "Shader Introduction", size: {1280, 720}, not full screen.
};

our::Application *app = new ShaderIntroductionApplication();
int width, height;
class PlayState : public GameState
{
    our::ShaderProgram shader, shader2;
    our::ShaderProgram *ptrShader,*terrainShader;
    our::Mesh cube_model;
    our::Mesh sun_model;
    our::Mesh plane_model;
    our::Mesh Girl_model;
    std::vector<Entity *> Entities;
    std::vector<Entity *> LightEntities;
//    std::vector<TransformComponent *> transforms;
//    std::vector<RenderState *> renderStates;
//    std::vector<MeshRendererComponent *> meshRenderers;
    std::vector<our::Mesh*> meshes;
    int mml = 0;
    float zoom = 1;
    Sampler *height_sampler;
    glm::mat4 cameraMatrix;
    Entity *CameraEntity = new Entity(NULL);
    CameraComponent *cameraComponent = new CameraComponent();
    CameraControllerComponent *cameraController = new CameraControllerComponent(app,cameraComponent);

    //////////////////////// Texture 2d and Sampler///////////////////////
    Texture2D* TextureObject = new Texture2D();
    Sampler* SampleObject = new Sampler();

    Texture2D* TextureObject2 = new Texture2D();

    //////////// CREATING LIGHTS AND ITS COMPONENTS////////////////////////////
    Entity *Light1 = new Entity(NULL);
    LightComponent *light = new LightComponent(LightType::DIRECTIONAL);
    TransformComponent * trlight = new TransformComponent(Light1);

    Entity *Light2 = new Entity(NULL);
    LightComponent *light2 = new LightComponent(LightType::POINT);
    TransformComponent * trlight2 = new TransformComponent(Light2);
///////////////////////////////////////////////////////////////////////////////////////
    ////////Adding Material
    Material * MaterialObj= new Material();
    Material * MaterialObj2= new Material();
    Material * MaterialObj3= new Material();
    Material * MaterialObj4= new Material();

    GameState *handleEvents()
    {
        if (app->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            return switchState(1);
        }
        return NULL;
    }

    void onEnter() override
    {
        ///////////////////// GENERATING METAL MATERIAL //////////////////////////
        std::vector<Texture2D *> Metal;
        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/metal/albedo.jpg", true);
        Metal.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/metal/specular.jpg", true);
        Metal.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/metal/roughness.jpg", true);
        Metal.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Metal.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Metal.push_back(TextureObject);


        ///////////////////// GENERATING WOOD MATERIAL //////////////////////////
        std::vector<Texture2D *> Wood;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/wood/albedo.jpg", true);
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/wood/specular.jpg", true);
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/wood/roughness.jpg", true);
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Wood.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Wood.push_back(TextureObject);


        ///////////////////// GENERATING ASPHALT MATERIAL //////////////////////////
        std::vector<Texture2D *> asphalt;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/albedo.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/specular.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/roughness.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/images/common/materials/asphalt/emissive.jpg", true);
        asphalt.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        asphalt.push_back(TextureObject);


        ///////////////////// GENERATING Girl MATERIAL //////////////////////////
        std::vector<Texture2D *> Girl;

        TextureObject = new Texture2D();
        TextureObject->ActivateTexture("assets/models/kawai/textures/face.jpg", true);
        Girl.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Girl.push_back(TextureObject);


        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Girl.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {0, 0, 0, 255});
        Girl.push_back(TextureObject);

        TextureObject = new Texture2D();
        glGenTextures(1, &(TextureObject->texture));
        our::texture_utils::singleColor(TextureObject->texture, {255, 255, 255, 255});
        Girl.push_back(TextureObject);


        //here change the texture
//        TextureObject->ActivateTexture("assets/images/common/monarch.png",true);
        SampleObject->InitializeSampler();

//        TextureObject2->ActivateTexture("assets/images/ex24_displacement/grass_ground_d.jpg", true);
//        int width, height;
        glfwGetFramebufferSize(app->window, &width, &height);


        shader.create();
        shader.attach("assets/shaders/light/light_transform.vert", GL_VERTEX_SHADER);
        shader.attach("assets/shaders/light/light_array.frag", GL_FRAGMENT_SHADER);
        shader.link();

        shader2.create();
        shader2.attach("assets/shaders/light/light_transform.vert", GL_VERTEX_SHADER);
        shader2.attach("assets/shaders/light/directional_light.frag", GL_FRAGMENT_SHADER);
        shader2.link();



        // FOR EACH ENTITY WE WILL USE IT ON ITS SHADER

        ptrShader = &shader;
        terrainShader = &shader2;
        //Camera Operations
        CameraEntity->addComponent(cameraComponent, "camera");
        CameraEntity->addComponent(cameraController, "controller");
        cameraMatrix = cameraComponent->getCameraMatrix();
        cameraComponent->setEyePosition({0, 0, 40});
        cameraComponent->setTarget({0, 0, 0});
        cameraComponent->setUp({0, 1, 0});
        cameraComponent->setupPerspective(glm::pi<float>()/2, static_cast<float>(width)/height, 0.1f, 100.0f);
        Entities.push_back(CameraEntity);


        //////////// INITIALIZING LIGHTS AND ITS COMPONENTS////////////////////////////
        trlight->init(app, {0, 1, 2}, {-1, -1, -1}, {1, 1, 1});
        Light1->addComponent(trlight, "transform");
        Light1->addComponent(light, "light");
        LightEntities.push_back(Light1);

        trlight2->init(app, {0, 1, 2}, {-1, -1, -1}, {1, 1, 1});
        Light2->addComponent(trlight2, "transform");
        Light2->addComponent(light2, "light");
        LightEntities.push_back(Light2);

        ////////////////Initializing Material/////////////////////////////
        // TODO pass texture and renderstate objects
        // TODO CHECK CULLING AND BLENDING
        RenderState * rs = new RenderState();
        rs->setDepthTesting(true,GL_LEQUAL);
        rs->setCulling(true,GL_BACK,GL_CCW);
        rs->setBlending(false);

        RenderState * rs2 = new RenderState();
        rs2->setDepthTesting(true,GL_LEQUAL);
        rs2->setCulling(false,GL_BACK,GL_CCW);
        rs2->setBlending(false);



        MaterialObj->init(ptrShader, Metal, rs, SampleObject);
//        MaterialObj->tint = {1,1,1,1};

        MaterialObj2->init(ptrShader, Wood, rs2, SampleObject);

        MaterialObj3->init(ptrShader, asphalt, rs2, SampleObject);

        MaterialObj4->init(ptrShader, Girl, rs2, SampleObject);

//        MaterialObj2->tint = {1,1,1,1};

        our::mesh_utils::Cuboid(cube_model, false);
        meshes.push_back(&cube_model);
        createObject({0, 0, 0}, {0, 0, 0}, {5, 5, 5}, cube_model, MaterialObj);


        our::mesh_utils::loadOBJ(Girl_model, "assets/models/kawai/Signora.obj");
        meshes.push_back(&Girl_model);
        createObject({2, 4, -3}, {0, 0, 5}, {0.01, 0.01, 0.01}, Girl_model, MaterialObj4);



        our::mesh_utils::Plane(plane_model, {1, 1}, false, {0, 0, 0},
                               {1, 1}, {0, 0}, {100, 100});
        height_sampler = new Sampler();
        meshes.push_back(&plane_model);
        createObject({0,0, -50}, {0, 0, 0}, {1000, 1000, 300}, plane_model,  MaterialObj2);


        our::mesh_utils::Sphere(sun_model, {32, 16}, false);
        meshes.push_back(&sun_model);
        createObject({7, 10, -7}, {0, 0, 0}, {5, 5, 5}, sun_model,  MaterialObj3);

//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        // The height texture won't be repeated so no need to use repeat.
//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glSamplerParameteri(height_sampler->getSampler(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glEnable(GL_DEPTH_TEST);
//        glDepthFunc(GL_LEQUAL);
//
//        glEnable(GL_CULL_FACE);
//        glCullFace(GL_BACK);
//        glFrontFace(GL_CCW);
        glClearColor(0, 0, 0, 0);

    }

    void onDraw(double deltaTime) override
    {

        glUseProgram(shader);
        for (unsigned int i = 0; i < Entities.size(); i++) {
            if (Entities[i]->getComponent("transform")) {
                TransformComponent *transComponent = ((TransformComponent *) Entities[i]->getComponent("transform"));
                transComponent->update(deltaTime);
            }
        }

        glm::vec3 cameraPos = cameraComponent->getEyePosition();
        std::sort(Entities.begin()+1, Entities.end(), [&cameraPos]
                (Entity * e1, Entity * e2) -> bool {
            if(e1->getComponent("mesh") && e2->getComponent("mesh")) {
                RenderState *state1 =((MeshRendererComponent *)e1->getComponent("mesh"))->Materialpt->renderstate;
                TransformComponent *tr1 = ((TransformComponent *) e1->getComponent("transform"));
                TransformComponent *tr2 = ((TransformComponent *) e2->getComponent("transform"));
                RenderState *state2 = ((MeshRendererComponent *)e2->getComponent("mesh"))->Materialpt->renderstate;
                if(state1->blend && state2->blend) {
                    float d1 = glm::length(tr1->position - cameraPos);
                    float d2 = glm::length(tr2->position - cameraPos);
                    if(d1 <= d2) {
                        return false;
                    }
                    else
                        return true;
                } else if(state1->blend && !state2->blend) {
                    return false;
                }
                else if(!state1->blend && state2->blend)
                    return true;
                else {
                    float d1 = glm::length(tr1->position - cameraPos);
                    float d2 = glm::length(tr2->position - cameraPos);
                    if(d1 <= d2) {
                        return true;
                    }
                    return false;
                }
            }

        });


        cameraController->update(deltaTime);
        shader.set("camera_position", cameraComponent->getEyePosition());
        shader.set("view_projection", cameraComponent->getVPMatrix());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int light_index = 0;
        const int MAX_LIGHT_COUNT = 16;
        for(const auto& e : LightEntities) {
            // GET LIGHT COMPONENT FROM ENTITY CONTAINING LIGHT
            LightComponent * light = ((LightComponent *)e->getComponent("light"));
            TransformComponent * transComponent = ((TransformComponent *)e->getComponent("transform"));

            if(!light->enabled) continue;
            std::string prefix = "lights[" + std::to_string(light_index) + "].";

            shader.set(prefix + "type", static_cast<int>(light->Type));
            shader.set(prefix + "color", {1,1,1});


            switch (light->Type) {
                case LightType::DIRECTIONAL:
                    shader.set(prefix + "direction", glm::normalize(transComponent->rotation));
                    break;
                case LightType::POINT:
                    shader.set(prefix + "position", transComponent->position);
                    shader.set(prefix + "attenuation_constant", light->attenuation.constant);
                    shader.set(prefix + "attenuation_linear", light->attenuation.linear);
                    shader.set(prefix + "attenuation_quadratic", light->attenuation.quadratic);
                    break;
                case LightType::SPOT:
                    shader.set(prefix + "position", transComponent->position);
                    shader.set(prefix + "direction", glm::normalize(transComponent->rotation));
                    shader.set(prefix + "attenuation_constant", light->attenuation.constant);
                    shader.set(prefix + "attenuation_linear", light->attenuation.linear);
                    shader.set(prefix + "attenuation_quadratic", light->attenuation.quadratic);
                    shader.set(prefix + "inner_angle", light->spot_angle.inner);
                    shader.set(prefix + "outer_angle", light->spot_angle.outer);
                    break;
            }
            light_index++;
            if(light_index >= MAX_LIGHT_COUNT) break;
        }
        // Since the light array in the shader has a constant size, we need to tell the shader how many lights we sent.
        shader.set("light_count", light_index);



//        //// new added here
//        TextureObject->LinkTexture();
//        SampleObject->AttachSample();
//
        shader.set("sampler", 0);
        shader.set("lod", mml);
        shader.set("zoom", zoom);

        cameraMatrix = cameraComponent->getCameraMatrix();


        for (unsigned int i = 0; i < Entities.size(); i++)
        {

            //// modified here
            if (Entities[i]->getComponent("mesh"))
            {
                TransformComponent *tr1 = ((TransformComponent *) Entities[i]->getComponent("transform"));
                float d1 = glm::length(tr1->position - cameraPos);
                RenderState *state1 =((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt->renderstate;
                bool bl = state1->blend;
//                our::ShaderProgram * S= ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt->MatrixShader;
                Material * material = ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt;
                Texture2D * albedo_map = material->albedo_map;
                Texture2D * specular_map =material->specular_map;
                Texture2D * roughness_map =material->roughness_map;
                Texture2D * emissive_map =material->emissive_map;
                Texture2D * ambient_occlusion_map =material->ambient_occlusion_map;
                Sampler * samp =material->sampler;
                
                //// BIND ALL SAMPLERS
                for(GLuint unit = 0; unit < 5; ++unit)
                    samp->AttachSample(unit);

//                S->set("camera_position", cameraComponent->getEyePosition());
//                S->set("view_projection", cameraComponent->getVPMatrix());
//                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                //// new added here
//                Text->LinkTexture();
//                samp->AttachSample();

//                S->set("sampler", 0);
//                S->set("lod", mml);
//                S->set("zoom", zoom);

                RenderState * state= ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->Materialpt->renderstate;
                if(!state->getDepthTesting())
                {
                    glDisable(GL_DEPTH_TEST);
                }
                else
                {
                    glEnable(GL_DEPTH_TEST);
                }
                if(!state->getCulling())
                {
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                    glEnable(GL_CULL_FACE);
                }

                TransformComponent* transComponent = ((TransformComponent *)Entities[i]->getComponent("transform"));
                glm::mat4 transformationMatrix = transComponent->to_mat4();
                glm::mat4 camTransformationMatrix =((CameraComponent *) Entities[0]->getComponent("camera"))->getCameraMatrix();


                shader.set("object_to_world", transformationMatrix);
                shader.set("object_to_world_inv_transpose", glm::inverse(transformationMatrix), true);
                shader.set("material.albedo_tint", material->albedo_tint);
                shader.set("material.specular_tint", material->specular_tint);
                shader.set("material.roughness_range", material->roughness_range);
                shader.set("material.emissive_tint", material->emissive_tint);

                albedo_map->LinkTexture(0);
                shader.set("material.albedo_map", 0);

                specular_map->LinkTexture(1);
                shader.set("material.specular_map", 1);

                ambient_occlusion_map->LinkTexture(2);
                shader.set("material.ambient_occlusion_map", 2);

                roughness_map->LinkTexture(3);
                shader.set("material.roughness_map", 3);

                emissive_map->LinkTexture(4);
                shader.set("material.emissive_map", 4);


//                shader.set("light.diffuse", light.diffuse);
//                shader.set("light.specular", light.specular);
//                shader.set("light.ambient", light.ambient);
//                // TODO SET LIGHT DIRECTION AND POSITION FROM TRANSFORM COMPONENT
////                shader.set("light.direction", glm::normalize(light.direction));
//                shader.set("material.diffuse",MaterialObj->diffuse);
//                shader.set("material.specular", MaterialObj->specular);
//                shader.set("material.ambient", MaterialObj->ambient);
//                shader.set("material.shininess", MaterialObj->shininess);
//                shader.set("object_to_world", transformationMatrix);
//                shader.set("object_to_world_inv_transpose", glm::inverse(transformationMatrix), true);
                transformationMatrix=transformationMatrix* glm::inverse(camTransformationMatrix);


                ((MeshRendererComponent *)Entities[i]->getComponent("mesh"))->draw(transformationMatrix, cameraMatrix);
            }

        }



    }
    void onExit() override
    {
        shader.destroy();
        for ( int i = 0; i<meshes.size(); i++) meshes[i]->destroy();
//        cube_model.destroy();
        cameraController->release();
    }

    void createObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, our::Mesh &mesh, Material * material)
    {
        Entity* ent = new Entity(nullptr);
        TransformComponent * ts = new TransformComponent(ent);
        ts->init(app, pos, rot, sc);
        MeshRendererComponent* mrc = new MeshRendererComponent();
        mrc->init(material, &mesh);
//        RenderState* rs = new RenderState();
//        renderStates.push_back(rs);
//        meshRenderers.push_back(mrc);
//        transforms.push_back(ts);
//        rs->setDepthTesting(true,GL_LEQUAL);
//        rs->setCulling(true,GL_BACK,GL_CCW);
//        rs->setBlending(false);
//        ent->addComponent(rs,"renderState");
        ent->addComponent(mrc, "mesh");
        ent->addComponent(ts, "transform");
        Entities.push_back(ent);
    }

};
PlayState *pState = new PlayState();

class MenuState : public GameState
{


public:

    GameState *handleEvents()
    {

        if (app->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            return switchState(2);
        }
    }
    void onEnter() override
    {
        program = glCreateProgram(); // We ask GL to create a program for us and return a uint that we will use it by.
        // (act as a pointer to the created program).

        attachShader(program, "assets/shaders/phase1.vert", GL_VERTEX_SHADER);   // read the vertex shader and attach it to the program.
        attachShader(program, "assets/shaders/phase1.frag", GL_FRAGMENT_SHADER); // read the fragment shader and attach it to the program.

        glLinkProgram(program);             // Link the vertex and fragment shader together.
        checkProgramLinkingErrors(program); // Check if there is any link errors between the fragment shader and vertex shader.

        glGenVertexArrays(1, &vertex_array); // Ask GL to create a vertex array to easily create a triangle.

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the clear color
    }
    void onDraw(double deltaTime) override
    {

        glClear(GL_COLOR_BUFFER_BIT);    // Clear the frame buffer (back buffer of the window)
        glUseProgram(program);           // Ask GL to use this program for the upcoming operations.
        // Every shader and rendering call after glUseProgram will now use this program object (and the shaders).
        glBindVertexArray(vertex_array); // Binding is like selecting which object to use.
        // Note that we need to bind a vertex array to draw
        // Even if that vertex array does not send any data down the pipeline

        //Following Code is for phase 1
        if (app->getMouse().justReleased(0))
        {
            if (app->getMouse().getMousePosition()[0] < (width/2))
                app->gotoState(pState);
            else {
                //TODO:
                glfwDestroyWindow(app->window);

            }
        }
        GLuint keyboard_uniform_location = glGetUniformLocation(program, "keyboard");
        glUniform1i(keyboard_uniform_location, keyShaderOption);

        mouse = app->getMouse().getMousePosition();
        GLuint mouse_uniform_location = glGetUniformLocation(program, "mouse");
        glUniform2f(mouse_uniform_location, mouse.x, mouse.y);
        // Sends vertices down the pipeline for drawing
        // Parameters:
        // mode (GLenum): what primitives to draw. GL_TRIANGLES will combine each 3 vertices into a triangle.
        // first (GLint): the index of the first vertex to draw. It is useless here since we are not receiving data through the vertex array.
        // count (GLsizei): How many vertices to send to the pipeline. Since we are sending 3 vertices only, only one triangle will be drawn.
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0); // Unbind the buffer.
    }
    void onExit() override
    {
        glDeleteProgram(program);               // Cleaning up the program we compiled and saved.
        glDeleteVertexArrays(1, &vertex_array); // Clean up the array we allocated Params: (n: number of vertex array objects, array containing the n addresses of the objects)
    }
};


MenuState *mState = new MenuState();

GameState *switchState(int state)
{
    if (state == 1) {
//        mState = new MenuState();
        return mState;
    }
//    pState = new PlayState();
    return pState;

    // TODO CHECK CULLING ON SWITCH
}



int main(int argc, char **argv)
{
    // ShaderIntroductionApplication().gotoState(menu);
    app->gotoState(mState);

    // return ShaderIntroductionApplication().run();
    return app->run();
}
