wall = {
    -- Graphic infos
    path     = "Game/assets/sprites/wall/wall.png",
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
    assert(instance.path ~= nil, "path is required")
    assert(instance.srcrect ~= nil, "srcrect is required")
    assert(instance.dstScale ~= nil, "dstScale is required")
    assert(instance.x ~= nil, "x is required")
    assert(instance.y ~= nil, "y is required")
    assert(instance.hitbox ~= nil, "hitbox is required")
    assert(instance.angle ~= nil, "angle is required")
    assert(instance.z ~= nil, "z is required")
    return instance
end

function wall:OnInit()
    -- Empty
end

function wall:OnUpdate(dt)
    -- Empty
end

function wall:OnCollision(tag, entityId, overlapX, overlapY, overlapW, overlapH)
    -- Empty
end

function wall:OnDestroy()
	--Empty
end
