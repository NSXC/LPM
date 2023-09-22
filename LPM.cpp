#include "lua.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <variant>

class LuaPluginManager {
public:
    LuaPluginManager() {
        luaState = luaL_newstate();
        luaL_openlibs(luaState);
    }

    ~LuaPluginManager() {
        lua_close(luaState);
    }
    lua_State* GetLuaState() {
        return luaState;
    }

    bool LoadPlugin(const std::string& pluginPath) {
        pluginPaths.push_back(pluginPath);
        return true;
    }

    void ExecutePlugins() {
        for (const auto& pluginPath : pluginPaths) {
            if (luaL_loadfile(luaState, pluginPath.c_str()) || lua_pcall(luaState, 0, 0, 0)) {
                const char* errorMessage = lua_tostring(luaState, -1);
                lua_pop(luaState, 1);
            }
        }
    }
    template<typename T>
    T GetLuaVariable(const std::string& varName) {
        lua_getglobal(luaState, varName.c_str());

        if (lua_isnumber(luaState, -1)) {
            if constexpr (std::is_same_v<T, int>) {
                if (lua_isinteger(luaState, -1)) {
                    T value = static_cast<T>(lua_tointeger(luaState, -1));
                    lua_pop(luaState, 1);
                    return value;
                }
            }

            if constexpr (std::is_same_v<T, double>) {
                T value = static_cast<T>(lua_tonumber(luaState, -1));
                lua_pop(luaState, 1);
                return value;
            }
        }
        else if (lua_isstring(luaState, -1)) {
            if constexpr (std::is_same_v<T, std::string>) {
                const char* str = lua_tostring(luaState, -1);
                lua_pop(luaState, 1);
                return std::string(str);
            }
        }

        lua_pop(luaState, 1);
        return T(); 
    }
    template<typename T>
    void SetLuaVariable(const std::string& varName, T value) {
        lua_pushnumber(luaState, static_cast<double>(value));
        lua_setglobal(luaState, varName.c_str());
    }
    template<typename Func>
    void RegisterFunction(const std::string& funcName, Func* func) {
        lua_pushlightuserdata(luaState, (void*)func);
        lua_pushcclosure(luaState, &LuaFunctionWrapper<Func>, 1);
        lua_setglobal(luaState, funcName.c_str());
    }

private:
    lua_State* luaState;
    std::vector<std::string> pluginPaths;
    template<typename Func>
    static int LuaFunctionWrapper(lua_State* L) {
        Func* func = (Func*)lua_touserdata(L, lua_upvalueindex(1));
        if (func) {
            int result = (*func)(L);
            lua_pushnumber(L, result);
            return 1;
        }
        return 0;
    }
};
