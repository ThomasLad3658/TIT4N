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
    ax = 0, ay = 0,
    angle = 0,
    srcrect  = { x = 0, y = 0, w = 100, h = 100 },
    dstScale = 5,

    -- Behavior infos
    hitbox  = { ox = 220, oy = 195, w = 60, h = 90 },

-- Custom properties (optional properties) do what you want here
    maxSpeed = 100,
    speed = 50,
    friction = 30,
    jumpStrength = 200,
    isGrounded = false
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
    local KeyA = GetKeyState("A")
    local KeyB = GetKeyState("D")
    local KeySpace = GetKeyState("Space")
    if KeyA == 2 or KeyA == 1 then
        self.ax = self.ax - self.speed * dt
    end
    if KeyB == 2 or KeyB == 1 then
        self.ax = self.ax + self.speed * dt
    end
    if (KeySpace == 2 or KeySpace == 1) and self.isGrounded then
        self.ay = self.ay + self.jumpStrength * dt
    end
    if KeyA == 0 and KeyB == 0 then
        if self.ax > 0 then
            self.ax = math.max(0, self.ax - self.friction * dt)
        else if self.ax < 0 then
                self.ax = math.min(0, self.ax + self.friction * dt)
            end
        end
    end

    self.ax = math.max(-self.maxSpeed, math.min(self.ax, self.maxSpeed))
    self.ay = math.max(-self.maxSpeed, math.min(self.ay, self.maxSpeed))

    self.ay = self.ay + 30 * dt

    self.dx = self.dx + self.ax
    self.dy = self.dy + self.ay

    self.x = self.x + self.dx
    self.y = self.y + self.dy
    
    self.isGrounded = false

    -- Animations
    if ax ~=0 then
        newAction = "Walk"
    end
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
            if overlapX + overlapW/2 > self.x + self.hitbox.ox + self.hitbox.w/2 then
                self.x = self.x - overlapW
                self.ax = math.min(self.ax, 0)
            else
                self.x = self.x + overlapW
                self.ax = math.max(self.ax, 0)
            end
        else
            if overlapY + overlapH/2 > self.y + self.hitbox.oy + self.hitbox.h/2 then
                self.y = self.y - overlapH
                self.isGrounded = true
                self.ay = math.min(self.ay, 0)
            else
                self.y = self.y + overlapH
                self.ay = math.max(self.ay, 0)
            end
        end
    elseif tag == "player" then
        if overlapW < overlapH then
            if overlapX + overlapW/2 > self.x + self.hitbox.ox + self.hitbox.w/2 then
                self.x = self.x - overlapW/2
            else
                self.x = self.x + overlapW/2
            end
        else
            if overlapY + overlapH/2 > self.y + self.hitbox.oy + self.hitbox.h/2 then
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