#include "gravity_app.h"

#include <tuple>

class GravityPhysicsSystem {
public:
    GravityPhysicsSystem(float strength) : strengthGravity{ strength } {}

    // const value like g in physics
    const float strengthGravity;

    // dt stands for delta time, and specifies the amount of time to advance the simulation
    // substeps is how many intervals to divide the forward time step in
    // more substeps result in a more stable simulation, but takes longer to compute
    void update(std::vector<GameObject>& objs, float dt, unsigned int substeps = 1) {
        // another delta time, but for delta step
        const float stepDelta = dt / substeps;
        for (int i = 0; i < substeps; i++)
            stepSimulation(objs, stepDelta);
    }

    // how strong they are 
    glm::vec2 computeForce(GameObject& fromObj, GameObject& toObj) const {
        // vector of distance between them
        auto offset = fromObj.transform2D.position - toObj.transform2D.position;
        // squared distance for optimization
        float distanceSquared = glm::dot(offset, offset);

        // clown town - just going to return 0 if objects are too close together...
        if (glm::abs(distanceSquared) < 1e-10f)
            return { .0f, .0f };

        // Classical Newton's law for gravity
        float force = strengthGravity * toObj.rigidbody2D.mass * fromObj.rigidbody2D.mass / distanceSquared;
        // This is shit code for faster simulation
        return force * offset / glm::sqrt(distanceSquared);
    }

private:
    // Core of physics simulation
    void stepSimulation(std::vector<GameObject>& physicsObjs, float dt) {
        // Loops through all pairs of objects and applies attractive force between them
        for (auto iterA = physicsObjs.begin(); iterA != physicsObjs.end(); ++iterA) {
            auto& objA = *iterA;
            for (auto iterB = iterA; iterB != physicsObjs.end(); ++iterB) {
                // Drop if same object
                if (iterA == iterB) continue;
                auto& objB = *iterB;

                auto force = computeForce(objA, objB);
                // Applying force for velocity as acceleration
                objA.rigidbody2D.velocity += dt * -force / objA.rigidbody2D.mass;
                objB.rigidbody2D.velocity += dt * force / objB.rigidbody2D.mass;
            }
        }

        // applying rigidbody velocity to transform position
        for (auto& obj : physicsObjs)
            obj.transform2D.position += dt * obj.rigidbody2D.velocity;
    }
};

class Vec2FieldSystem {
public:
    void update(const GravityPhysicsSystem& physicsSystem,
        std::vector<GameObject>& physicsObjs, std::vector<GameObject>& vectorField) {
        // For each field line we caluclate the net graviation force for that point in space
        for (auto& vf : vectorField) {
            // Calculate arithmetic mean for vectors of gravity
            glm::vec2 direction{};
            for (auto& obj : physicsObjs)
                direction += physicsSystem.computeForce(obj, vf);

            // This scales the length of the field line based on the log of the length
            // values were chosen just through trial and error based on what i liked the look of
            vf.transform2D.scale.x = 0.005f + 0.045f * glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 1.f);
            // Look into the direction vector
            vf.transform2D.rotation = atan2(direction.y, direction.x);
        }
    }
};

std::unique_ptr<VulkanModel> createSquareModel(VulkanDevice& device, glm::vec2 offset) {
    // 6 vertexes, 2 triangles, 1 square
    std::vector<VulkanModel::Vertex> vertices = {
        {{-0.5f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}},
        {{-0.5f, -0.5f}}, {{0.5f, -0.5f}}, {{0.5f, 0.5f}},
    };

    // And offset for customization
    for (auto& v : vertices)
        v.position += offset;
    
    return std::make_unique<VulkanModel>(device, vertices);
}

std::unique_ptr<VulkanModel> createCircleModel(VulkanDevice& device, unsigned int numSides) {
    // Generate points around circle
    std::vector<VulkanModel::Vertex> uniqueVertices{};
    for (int i = 0; i < numSides; i++) {
        float angle = i * glm::two_pi<float>() / numSides;
        uniqueVertices.push_back({ {glm::cos(angle), glm::sin(angle)} });
    }
    uniqueVertices.push_back({});  // adds center vertex at 0, 0

    // Generate triangles from points
    std::vector<VulkanModel::Vertex> vertices{};
    for (int i = 0; i < numSides; i++) {
        vertices.push_back(uniqueVertices[i]);
        vertices.push_back(uniqueVertices[(i + 1) % numSides]);
        vertices.push_back(uniqueVertices[numSides]);
    }

    return std::make_unique<VulkanModel>(device, vertices);
}

VulkanPhysicsApp::VulkanPhysicsApp(VulkanPhysicsAppSettings& settings)
{
    window = std::make_unique<VulkanWindow>(settings.screenWidth, settings.screenHeight, settings.name);
    device = std::make_unique<VulkanDevice>(*window);
    renderer = std::make_unique<VulkanRenderer>(*window, *device);
    renderSystem = std::make_unique<SimpleRenderSystem>(*device,
        renderer->getSwapChainRenderPass(),
        settings.vertShaderPath, settings.fragShaderPath);
}
VulkanPhysicsApp::~VulkanPhysicsApp()
{

}

void VulkanPhysicsApp::run()
{
    GravityPhysicsSystem gravitySystem{ 0.81f };
    Vec2FieldSystem vecFieldSystem{};

    auto physicsObjects = createPhysicsObjects();
    auto vectorField = createVectorField();

    while (!window->shouldClose()) {
        glfwPollEvents();

        if (auto commandBuffer = renderer->beginFrame()) {
            // update systems
            gravitySystem.update(physicsObjects, 1.0f / 60, 5);
            vecFieldSystem.update(gravitySystem, physicsObjects, vectorField);

            // render system
            renderer->beginSwapChainRenderPass(commandBuffer);
            //renderSystem->renderGameObjects(commandBuffer, gameObjects);
            renderSystem->renderGameObjects(commandBuffer, physicsObjects);
            renderSystem->renderGameObjects(commandBuffer, vectorField);
            renderer->endSwapChainRenderPass(commandBuffer);
            renderer->endFrame();
        }
    }
    // Fix errors while dispose Vulkan
    vkDeviceWaitIdle(device->device());
}

std::vector<GameObject> VulkanPhysicsApp::createPhysicsObjects() {
    std::shared_ptr<VulkanModel> circleModel = createCircleModel(*device.get(), 64);

    std::vector<GameObject> physicsObjects{};

    auto red = GameObject::createGameObject();
    red.transform2D.scale = glm::vec2{ .02f };
    red.transform2D.position = { .5f, .5f };
    red.color = { 1.f, 0.f, 0.f };
    red.rigidbody2D.velocity = { -.5f, .0f };
    red.model = circleModel;
    physicsObjects.push_back(red);

    auto blue = GameObject::createGameObject();
    blue.transform2D.scale = glm::vec2{ .02f };
    blue.transform2D.position = { -.45f, -.25f };
    blue.color = { 0.f, 0.f, 1.f };
    blue.rigidbody2D.velocity = { .5f, .0f };
    blue.model = circleModel;
    physicsObjects.push_back(blue);

    return physicsObjects;
}

std::vector<GameObject> VulkanPhysicsApp::createVectorField() {
    // offset model by .5 so rotation occurs at edge rather than center of square
    std::shared_ptr<VulkanModel> squareModel = createSquareModel(*device.get(), { .5f, .0f });

    std::vector<GameObject> vectorField{};
    int gridCount = 40;
    for (int i = 0; i < gridCount; i++) {
        for (int j = 0; j < gridCount; j++) {
            auto vf = GameObject::createGameObject();
            vf.transform2D.scale = glm::vec2(0.005f);
            vf.transform2D.position = {
                -1.0f + (i + 0.5f) * 2.0f / gridCount,
                -1.0f + (j + 0.5f) * 2.0f / gridCount };
            vf.color = glm::vec3(1.0f);
            vf.model = squareModel;
            vectorField.push_back(std::move(vf));
        }
    }

    return vectorField;
}