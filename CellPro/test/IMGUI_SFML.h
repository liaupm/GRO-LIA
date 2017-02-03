
namespace sf
{
    class Window;
    class Event;
}

namespace ImGui
{
    namespace SFML
    {
        void Init(sf::Window& window);
        void ProcessEvent(const sf::Event& event);
        void Update(sf::Window& window, float dt);
        void Shutdown();
    }
}
