-- script.lua
print("Hello from Lua!")

-- Call the C++ function 'add' that our program registered.
local result = add(10, 5)
print("The result of calling add(10, 5) from Lua is: " .. result)

-- Let's try it with floating point numbers.
local result2 = add(3.14, 2.86)
print("add(3.14, 2.86) = " .. result2)

-- This call will trigger the error handling we added in C++.
print("Attempting to call add with wrong arguments...")
local ok, err = pcall(add, "hello", "world")
if not ok then
    print("Lua caught an error: " .. err)
end