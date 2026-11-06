
#include <cstdio>
#include <cstdint>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <vector>
#include <string>
#include <algorithm>

#include "../PhysicsBody.h"
#include "../Rectangle.h"
#include "../Circle.h"
#include "../BoxCollider.h"

enum class ObjectType
{
    Rectangle,
    Circle
};

struct GameObject
{
    std::string name;
    ObjectType type = ObjectType::Rectangle;

    Rectangle rectangle;
    Circle circle;

    BoxCollider collider;

    bool hasCollider = true;
    bool physicsEnabled = true;

    sf::Vector2f startPosition{0.f, 0.f};

    PhysicsBody& GetPhysics()
    {
        if (type == ObjectType::Rectangle)
            return rectangle.physics;

        return circle.physics;
    }

    const PhysicsBody& GetPhysics() const
    {
        if (type == ObjectType::Rectangle)
            return rectangle.physics;

        return circle.physics;
    }

    sf::Vector2f GetPosition() const
    {
        if (type == ObjectType::Rectangle)
            return rectangle.shape.getPosition();

        return circle.shape.getPosition();
    }

    void SetPosition(sf::Vector2f position)
    {
        rectangle.shape.setPosition(position);
        circle.shape.setPosition(position);

        SyncCollider();
    }

    sf::Vector2f GetSize() const
    {
        if (type == ObjectType::Rectangle)
            return rectangle.shape.getSize();

        float diameter = circle.shape.getRadius() * 2.f;
        return {diameter, diameter};
    }

    void SetSize(sf::Vector2f size)
    {
        rectangle.shape.setSize(size);

        float diameter = std::min(size.x, size.y);
        circle.shape.setRadius(diameter / 2.f);

        SyncCollider();
    }

    sf::Color GetColor() const
    {
        if (type == ObjectType::Rectangle)
            return rectangle.shape.getFillColor();

        return circle.shape.getFillColor();
    }

    void SetColor(sf::Color color)
    {
        rectangle.shape.setFillColor(color);
        circle.shape.setFillColor(color);
    }

    void SyncCollider()
    {
        collider.SetPosition(GetPosition());
        collider.SetSize(GetSize());
    }

    void Update(float deltaTime)
    {
        if (!physicsEnabled)
            return;

        if (type == ObjectType::Rectangle)
            rectangle.Update(deltaTime);
        else
            circle.Update(deltaTime);

        SyncCollider();
    }

    void Draw(sf::RenderTarget& target)
    {
        if (type == ObjectType::Rectangle)
            rectangle.Draw(target);
        else
            circle.Draw(target);
    }
};

struct ConsoleMessage
{
    std::string message;
    int type = 0;
};

std::vector<GameObject> objects;
std::vector<ConsoleMessage> consoleMessages;

int selectedObject = -1;

bool playing = false;
bool paused = false;

void Log(const std::string& message, int type = 0)
{
    consoleMessages.push_back({message, type});
}

void AddRectangle()
{
    GameObject object;

    object.name = "Rectangle";
    object.type = ObjectType::Rectangle;

    object.rectangle.shape.setSize({100.f, 100.f});
    object.rectangle.shape.setFillColor(sf::Color::Blue);

    object.SetPosition({100.f, 100.f});
    object.startPosition = object.GetPosition();

    object.SyncCollider();

    objects.push_back(object);
    selectedObject = static_cast<int>(objects.size()) - 1;

    Log("Rectangle created.");
}

void AddCircle()
{
    GameObject object;

    object.name = "Circle";
    object.type = ObjectType::Circle;

    object.circle.shape.setRadius(50.f);
    object.circle.shape.setFillColor(sf::Color::Red);

    object.SetPosition({250.f, 100.f});
    object.startPosition = object.GetPosition();

    object.SyncCollider();

    objects.push_back(object);
    selectedObject = static_cast<int>(objects.size()) - 1;

    Log("Circle created.");
}

void AddGround()
{
    GameObject object;

    object.name = "Ground";
    object.type = ObjectType::Rectangle;

    object.rectangle.shape.setSize({800.f, 40.f});
    object.rectangle.shape.setFillColor(sf::Color(90, 90, 90));

    object.SetPosition({0.f, 0.f});

    object.physicsEnabled = false;
    object.hasCollider = true;

    object.startPosition = object.GetPosition();

    object.SyncCollider();

    objects.push_back(object);

    Log("Ground created.");
}

void ResolveCollisions()
{
    for (std::size_t i = 0; i < objects.size(); ++i)
    {
        for (std::size_t j = i + 1; j < objects.size(); ++j)
        {
            GameObject& a = objects[i];
            GameObject& b = objects[j];

            if (!a.hasCollider || !b.hasCollider)
                continue;

            if (!a.collider.CheckCollision(b.collider))
                continue;

            bool movableA = a.physicsEnabled;
            bool movableB = b.physicsEnabled;

            if (!movableA && !movableB)
                continue;

            sf::Vector2f posA = a.GetPosition();
            sf::Vector2f posB = b.GetPosition();

            sf::Vector2f sizeA = a.GetSize();
            sf::Vector2f sizeB = b.GetSize();

            float overlapLeft =
                (posA.x + sizeA.x) - posB.x;

            float overlapRight =
                (posB.x + sizeB.x) - posA.x;

            float overlapTop =
                (posA.y + sizeA.y) - posB.y;

            float overlapBottom =
                (posB.y + sizeB.y) - posA.y;

            float overlapX = std::min(overlapLeft, overlapRight);
            float overlapY = std::min(overlapTop, overlapBottom);

            if (overlapX < overlapY)
            {
                float direction = posA.x < posB.x ? -1.f : 1.f;

                if (movableA && movableB)
                {
                    posA.x += direction * overlapX / 2.f;
                    posB.x -= direction * overlapX / 2.f;
                }
                else if (movableA)
                {
                    posA.x += direction * overlapX;
                }
                else
                {
                    posB.x -= direction * overlapX;
                }

                if (movableA)
                    a.GetPhysics().velocityX = 0.f;

                if (movableB)
                    b.GetPhysics().velocityX = 0.f;
            }
            else
            {
                float direction = posA.y < posB.y ? -1.f : 1.f;

                if (movableA && movableB)
                {
                    posA.y += direction * overlapY / 2.f;
                    posB.y -= direction * overlapY / 2.f;
                }
                else if (movableA)
                {
                    posA.y += direction * overlapY;
                }
                else
                {
                    posB.y -= direction * overlapY;
                }

                if (movableA)
                    a.GetPhysics().velocityY = 0.f;

                if (movableB)
                    b.GetPhysics().velocityY = 0.f;
            }

            a.SetPosition(posA);
            b.SetPosition(posB);
        }
    }
}

void DrawExplorer()
{
    ImGui::SetNextWindowPos({10.f, 10.f}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({220.f, 480.f}, ImGuiCond_FirstUseEver);

    ImGui::Begin("Explorer");

    if (ImGui::Button("+ Rectangle"))
        AddRectangle();

    ImGui::SameLine();

    if (ImGui::Button("+ Circle"))
        AddCircle();

    if (ImGui::Button("+ Ground"))
        AddGround();

    ImGui::Separator();
    ImGui::Text("Scene Objects");

    for (int i = 0; i < static_cast<int>(objects.size()); ++i)
    {
        ImGui::PushID(i);

        if (ImGui::Selectable(
            objects[i].name.c_str(),
            selectedObject == i))
        {
            selectedObject = i;
        }

        ImGui::PopID();
    }

    if (selectedObject >= 0 &&
        selectedObject < static_cast<int>(objects.size()))
    {
        ImGui::Separator();

        if (ImGui::Button("Delete Selected"))
        {
            Log(objects[selectedObject].name + " deleted.");

            objects.erase(objects.begin() + selectedObject);
            selectedObject = -1;
        }
    }

    ImGui::End();
}

void DrawInspector()
{
    ImGui::SetNextWindowPos({950.f, 90.f}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({310.f, 690.f}, ImGuiCond_FirstUseEver);

    ImGui::Begin("Inspector");

    if (selectedObject < 0 ||
        selectedObject >= static_cast<int>(objects.size()))
    {
        ImGui::TextDisabled("No object selected.");
        ImGui::End();
        return;
    }

    GameObject& object = objects[selectedObject];

    char nameBuffer[128];

    std::snprintf(
        nameBuffer,
        sizeof(nameBuffer),
        "%s",
        object.name.c_str()
    );

    if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
        object.name = nameBuffer;

    ImGui::Separator();
    ImGui::Text("Transform");

    sf::Vector2f position = object.GetPosition();

    float pos[2] = {position.x, position.y};

    if (ImGui::DragFloat2("Position", pos, 1.f))
        object.SetPosition({pos[0], pos[1]});

    sf::Vector2f size = object.GetSize();

    float dimensions[2] = {size.x, size.y};

    if (ImGui::DragFloat2("Size", dimensions, 1.f, 1.f, 1000.f))
        object.SetSize({dimensions[0], dimensions[1]});

    ImGui::Separator();
    ImGui::Text("Appearance");

    sf::Color color = object.GetColor();

    float colorValues[4] =
    {
        color.r / 255.f,
        color.g / 255.f,
        color.b / 255.f,
        color.a / 255.f
    };

    if (ImGui::ColorEdit4("Color", colorValues))
    {
        object.SetColor({
            static_cast<std::uint8_t>(colorValues[0] * 255.f),
            static_cast<std::uint8_t>(colorValues[1] * 255.f),
            static_cast<std::uint8_t>(colorValues[2] * 255.f),
            static_cast<std::uint8_t>(colorValues[3] * 255.f)
        });
    }

    ImGui::Separator();
    ImGui::Text("Physics");

    ImGui::Checkbox("Enable Physics", &object.physicsEnabled);

    PhysicsBody& physics = object.GetPhysics();

    ImGui::DragFloat("Gravity", &physics.gravity, 1.f, 0.f, 5000.f);
    ImGui::DragFloat("Velocity X", &physics.velocityX, 1.f);
    ImGui::DragFloat("Velocity Y", &physics.velocityY, 1.f);

    ImGui::Separator();
    ImGui::Text("Collision");

    if (ImGui::Checkbox("Has Collider", &object.hasCollider))
        Log(object.hasCollider ? "Collider enabled." : "Collider disabled.");

    ImGui::End();
}

void DrawConsole()
{
    ImGui::SetNextWindowPos({10.f, 500.f}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({220.f, 280.f}, ImGuiCond_FirstUseEver);

    ImGui::Begin("Console");

    if (ImGui::Button("Clear"))
        consoleMessages.clear();

    ImGui::Separator();

    ImGui::BeginChild("ConsoleOutput");

    for (const auto& message : consoleMessages)
    {
        if (message.type == 2)
            ImGui::TextColored({1.f, 0.3f, 0.3f, 1.f}, "[ERROR] %s", message.message.c_str());
        else if (message.type == 1)
            ImGui::TextColored({1.f, 0.8f, 0.2f, 1.f}, "[WARNING] %s", message.message.c_str());
        else
            ImGui::Text("[INFO] %s", message.message.c_str());
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 5.f)
        ImGui::SetScrollHereY(1.f);

    ImGui::EndChild();
    ImGui::End();
}

void DrawToolbar()
{
    ImGui::SetNextWindowPos({240.f, 10.f}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({1020.f, 70.f}, ImGuiCond_FirstUseEver);

    ImGui::Begin("Toolbar");

    if (!playing)
    {
        if (ImGui::Button("Play"))
        {
            playing = true;
            paused = false;

            Log("Game started.");
        }
    }
    else
    {
        if (ImGui::Button("Stop"))
        {
            playing = false;
            paused = false;

            for (auto& object : objects)
            {
                object.SetPosition(object.startPosition);

                PhysicsBody& physics = object.GetPhysics();
                physics.velocityX = 0.f;
                physics.velocityY = 0.f;
            }

            Log("Game stopped.");
        }

        ImGui::SameLine();

        if (ImGui::Button(paused ? "Resume" : "Pause"))
        {
            paused = !paused;
            Log(paused ? "Game paused." : "Game resumed.");
        }
    }

    ImGui::End();
}

void DrawGameView(sf::RenderTexture& scene)
{
    ImGui::SetNextWindowPos({240.f, 90.f}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({700.f, 690.f}, ImGuiCond_FirstUseEver);

    ImGui::Begin("Game View");

    ImVec2 available = ImGui::GetContentRegionAvail();

    scene.clear(sf::Color(45, 45, 50));

    for (auto& object : objects)
        object.Draw(scene);

    scene.display();

    sf::Vector2u sceneSize = scene.getSize();

    float sceneWidth = static_cast<float>(sceneSize.x);
    float sceneHeight = static_cast<float>(sceneSize.y);

    if (available.x > 0.f && available.y > 0.f)
    {
        float scale = std::min(
            available.x / sceneWidth,
            available.y / sceneHeight
        );

        ImVec2 imageSize(
            sceneWidth * scale,
            sceneHeight * scale
        );

        ImVec2 imagePosition = ImGui::GetCursorPos();

        imagePosition.x += (available.x - imageSize.x) * 0.5f;
        imagePosition.y += (available.y - imageSize.y) * 0.5f;

        ImGui::SetCursorPos(imagePosition);

        ImGui::Image(
            scene.getTexture(),
            sf::Vector2f{imageSize.x, imageSize.y}
        );
    }

    ImGui::End();
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1280, 800}),
        "2D Game Engine"
    );

    window.setFramerateLimit(60);

    sf::RenderTexture scene({800, 600});

    if (!ImGui::SFML::Init(window))
        return -1;

    ImGui::GetIO().IniFilename = nullptr;

    AddRectangle();
    AddCircle();
    AddGround();

    Log("Engine initialised.");
    Log("Scene loaded.");

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        ImGui::SFML::Update(window, deltaTime);

        float dt = std::min(deltaTime.asSeconds(), 0.05f);

        if (playing && !paused)
        {
            for (auto& object : objects)
                object.Update(dt);

            ResolveCollisions();
        }

        DrawToolbar();
        DrawExplorer();
        DrawInspector();
        DrawGameView(scene);
        DrawConsole();

        window.clear(sf::Color(30, 30, 30));

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
