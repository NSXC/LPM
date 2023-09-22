function execute_command(command)
    local handle = io.popen(command)
    local result = handle:read("*a")
    handle:close()
    return result
end
function ostype()
  local windows_version_info = execute_command("ver")
  return windows_version_info
end
