wall = {
    -- Graphic infos
    path     = "assets/sprites/wall/wall.png",
    mirroredH = false, mirroredV = false,
    visible  = true,

    -- Position infos
    x = 0, y = 0, z = 0,
    angle = 0,
    srcrect  = { x = 0, y = 0, w = 320, h = 320 },
    dstScale = 1,

    -- Behavior infos
    hitbox  = { ox = 0, oy = 0, w = 320, h = 320 },
}

wall.__index = wall

function wall:new(overrides)
	local instance = {}
    setmetatable(instance, wall)
    if overrides then
        for key, value in pairs(overrides) do
            instance[key] = value
        end
    end
    return instance
end
