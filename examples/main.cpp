#include "LPM.cpp"


int Multiply(lua_State* L) {
    int a = static_cast<int>(lua_tonumber(L, 1));
    int b = static_cast<int>(lua_tonumber(L, 2));
    int result = a * b;
    lua_pushnumber(L, result);
    return 1; 
}

int main() {
    LuaPluginManager pluginManager;
    pluginManager.RegisterFunction("Multiply", &Multiply);
    if (pluginManager.LoadPlugin("plugin.lua")) {
        std::cout << "Lua script loaded successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to load Lua script." << std::endl;
        return 1; 
    }
    pluginManager.ExecutePlugins();
    lua_State* L = pluginManager.GetLuaState();
    lua_getglobal(L, "Greet");
    if (lua_isfunction(L, -1)) {
        std::cout << "Greet function found in Lua." << std::endl;
        lua_pushstring(L, "John Doe");
        if (lua_pcall(L, 1, 1, 0) == LUA_OK) {
            const char* greeting = lua_tostring(L, -1);
            std::cout << "Greeting from Lua: " << greeting << std::endl;
            lua_pop(L, 1);
        }
        else {
            std::cerr << "Error calling Greet function in Lua." << std::endl;
        }
    }
    else {
        std::cout << "Greet function not found in Lua." << std::endl;
    }

    int intValue = pluginManager.GetLuaVariable<int>("pluginVarInt");
    double doubleValue = pluginManager.GetLuaVariable<double>("pluginVarDouble");
    std::string stringValue = pluginManager.GetLuaVariable<std::string>("pluginVarString");

    std::cout << "Modified Lua integer variable: " << intValue << std::endl;
    std::cout << "Modified Lua double variable: " << doubleValue << std::endl;
    std::cout << "Modified Lua string variable: " << stringValue << std::endl;

    return 0;
}
