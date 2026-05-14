player = {
-- Generic properties ( all required)
    -- Graphic infos
    path     = "Game/assets/sprites/player/Soldier.png",
    action = "Idle",
    mirroredH = false, mirroredV = false,
    visible  = true,
    animations = {
        Idle = { row = 0, frameCount = 6, fps = 9, loop = true  },
        Walk = { row = 1, frameCount = 8, fps = 8, loop = true  },
        Attack = { row = 2, frameCount = 6, fps = 12, loop = false }
    },

    -- Position infos
    x = 0, y = 0, z = 0,
    dx = 0, dy = 0,
    angle = 0,
    srcrect  = { x = 0, y = 0, w = 100, h = 100 },
    dstScale = 5,

    -- Behavior infos
    hitbox  = { ox = 220, oy = 195, w = 60, h = 90 },

-- Custom properties (optional properties) do what you want here
    hp    = 100,
    speed = 100
}

player.__index = player

function player:new(overrides)
	local instance = {}
    setmetatable(instance, player)
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

function player:OnInit()
	self.Play(self.action)
end

function player:OnUpdate(dt)
    local newAction = "Idle"
    self.dx = 0
    self.dy = 0

    -- Movements
    if GetKeyState("W") == 2 or GetKeyState("W") == 1 then
        self.dy = self.dy - self.speed * dt
        newAction = "Walk"
    end
    if GetKeyState("S") == 2 or GetKeyState("S") == 1 then
        self.dy = self.dy + self.speed * dt
        newAction = "Walk"
    end
    if GetKeyState("A") == 2 or GetKeyState("A") == 1 then
        self.dx = self.dx - self.speed * dt
        newAction = "Walk"
    end
    if GetKeyState("D") == 2 or GetKeyState("D") == 1 then
        self.dx = self.dx + self.speed * dt
        newAction = "Walk"
    end
    self.x = self.x + self.dx
    self.y = self.y + self.dy
    
    -- Animations
    if self.dx < 0 then
        self.mirroredH = true
    else if self.dx > 0 then
            self.mirroredH = false
        end
    end
    if newAction ~= self.action then
        self.action = newAction
        self.Play(self.action)
    end
end

function player:OnCollision(tag, overlapX, overlapY, overlapW, overlapH)
    if tag == "wall" then
        if overlapW < overlapH then
            if self.dx > 0 then
                self.x = self.x - overlapW
            else
                self.x = self.x + overlapW
            end
        else
            if self.dy > 0 then
                self.y = self.y - overlapH
            else
                self.y = self.y + overlapH
            end
        end
    elseif tag == "enemy" then
        if overlapW < overlapH then
            if self.dx > 0 then
                self.x = self.x - overlapW/2
            else
                self.x = self.x + overlapW/2
            end
        else
            if self.dy > 0 then
                self.y = self.y - overlapH/2
            else
                self.y = self.y + overlapH/2
            end
        end
    end
end

function player:OnDestroy()
	--Empty
end

--[[
    -- Required -> they will cause a crash if not specified
    path
    x, y
    srcrect
    dstScale
    hitbox
    angle       -> Can cause unexpected behaviour if not specified
    z           -> Can cause unexpected behaviour if not specified
    All 5 functions

    -- Optional -> they might cause unexpected behaviour (nil) if not specified or crash in some cases, but they are not required
    mirroredH   -> if not specified, they are set to false by default
    mirroredV   -> if not specified, they are set to false by default
    visible     -> if not specified, it is set to false by default
    animations  -> Crash if PlayAnimation is called without this table
    action      -> to help you manage your animations, not used by the engine, but useful for your scripts
    dx, dy      -> to help you manage your movements, not used by the engine, but useful for your scripts

    More functions and properties can be added as you want, they will be ignored by the engine if not used, but they can be useful for your scripts
]]