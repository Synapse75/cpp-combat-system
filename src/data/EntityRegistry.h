#include <unordered_map>
#include <string>

class EntityRegistry {
public:
    static EntityRegistry& Instance() {
        static EntityRegistry instance;
        return instance;
    }

    void Register(int id, const std::string& name) {
        registry_[id] = name;
    }

    std::string GetName(int id) const {
        auto it = registry_.find(id);
        return it != registry_.end() ? it->second : "Unknown";
    }

private:
    std::unordered_map<int, std::string> registry_;
};