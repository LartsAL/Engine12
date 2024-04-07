template<class T>
auto EngineConfig::get(std::string&& key) -> T {
    return std::any_cast<T>(vars[key]);
}

template<class T>
auto EngineConfig::set(std::string&& key, T&& value) -> void {
    vars[key] = std::any(value);
}