inVarInt = 42
pluginVarDouble = 3.14
pluginVarString = "Hello from Lua!"
print("Plugin Loaded")
function Multiply(a, b)
    return a * b
end

function Greet(name)
    return "Hello, " .. name .. "!"
end
