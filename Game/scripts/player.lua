player = {
-- Generic properties ( all required)
    -- Graphic infos
    path     = "Game/assets/sprites/player/Soldier.png",
    action = "Idle",
    mirroredH = false, mirroredV = false,
    visible  = true,
    animations = {
        Idle = { row = 0, frameCount = 6, fps = 8, loop = true  },
        Walk = { row = 1, frameCount = 8, fps = 8, loop = true  },
        Attack = { row = 2, frameCount = 6, fps = 12, loop = false },
        dead = { row = 6, frameCount = 4, fps = 4, loop = false }
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
    hp = 100,
    maxSpeed = 8,
    speed = 20,
    friction = 30,
    jumpStrength = 8,
    isGrounded = false,
    timeBeforeSuicide = 1,
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
    local newAction

    -- Movements
    local KeyA = GetKeyState("A")
    local KeyB = GetKeyState("D")
    local KeySpace = GetKeyState("Space")

    if KeyA == 2 or KeyA == 1 then
        self.dx = self.dx - self.speed * dt
    end
    if KeyB == 2 or KeyB == 1 then
        self.dx = self.dx + self.speed * dt
    end
    if KeySpace == 2 and self.isGrounded then
        self.dy = self.dy - self.jumpStrength
    end

    if KeyA == 0 and KeyB == 0 then
        if self.dx > 0 then
            self.dx = math.max(0, self.dx - self.friction * dt)
        elseif self.dx < 0 then
            self.dx = math.min(0, self.dx + self.friction * dt)
        end
    end

    self.dx = math.max(-self.maxSpeed, math.min(self.dx, self.maxSpeed))

    if self.hp <= 0 then
        self.dx = 0
        self.dy = 0
    end

    self.dy = self.dy + 30 * dt

    self.x = self.x + self.dx
    self.y = self.y + self.dy
    
    self.isGrounded = false

    -- Check life
    if self.hp <= 0 then
        if self.timeBeforeSuicide <= 0 then
            --self.Suicide()
        else
            self.timeBeforeSuicide = self.timeBeforeSuicide - dt
        end
    end

    -- Animations
    if self.dx ~= 0 then
        newAction = "Walk"
    else
        newAction = "Idle"
    end
    if self.hp <= 0 then
        newAction = "dead"
    end
    if self.dx < 0 then
        self.mirroredH = true
    elseif self.dx > 0 then
        self.mirroredH = false
    end
    if newAction ~= self.action then
        self.action = newAction
        self.Play(self.action)
    end
end

function player:fullCollision(overlapX, overlapY, overlapW, overlapH)
    if overlapW < overlapH then
        if overlapX + overlapW/2 > self.x + self.hitbox.ox + self.hitbox.w/2 then
            self.x = self.x - overlapW
            self.dx = math.min(self.dx, 0)
        else
            self.x = self.x + overlapW
            self.dx = math.max(self.dx, 0)
        end
    else
        if overlapY + overlapH/2 > self.y + self.hitbox.oy + self.hitbox.h/2 then
            self.y = self.y - overlapH
            self.isGrounded = true
            self.dy = math.min(self.dy, 0)
        else
            self.y = self.y + overlapH
            self.dy = math.max(self.dy, 0)
        end
    end
end

function player:partialCollision(overlapX, overlapY, overlapW, overlapH)
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

function player:OnCollision(tag, entityId, overlapX, overlapY, overlapW, overlapH)
    if tag == "wall" then
        self:fullCollision(overlapX, overlapY, overlapW, overlapH)
    elseif tag == "player" then
        self:partialCollision(overlapX, overlapY, overlapW, overlapH)
    elseif tag == "orc" then
        self:partialCollision(overlapX, overlapY, overlapW, overlapH)
        if GetEntityBool(entityId, "visible") then
            self.hp = self.hp - 5
        end
    end
end

function player:OnDestroy()
	--Empty
end

--[[
    -- Required   -> They can cause a crash if not specified
    x, y          -> Required
    srcrect       -> Required
    dstScale      -> Required
    hitbox        -> Required
    visible       -> Can be nil if path = "" (will be set to false)
    angle         -> Can be nil if path = "" or visible = false (will be set to 0)
    z             -> Can be nil if path = "" or visible = false (will be set to 0)
    mirroredH     -> Can be nil if path = "" or visible = false (will be set to false)
    mirroredV     -> Can be nil if path = "" or visible = false (will be set to false)
    animations    -> Crash if Play() is called without this table
    new()         -> Never modify, never remove, don't touch it
    OnInit()      -> Can be empty
    OnUpdate()    -> Can be empty
    OnCollision() -> Can be empty
    OnDestroy()   -> Can be empty

    -- Optional   -> Used by the engine if specified, but won't cause a crash if not specified
    path          -> Will be set to "" if not specified, and the entity will be invisible
    name		  -> Will be set to "" if not specified

    -- Custom     -> Custom properties and functions for your entities, they will be ignored by the engine
    action        -> to help you manage your animations
    dx, dy        -> to help you manage your movements
    Anything      -> More useful functions and properties can be added, as much as you want
]]