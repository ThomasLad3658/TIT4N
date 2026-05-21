orc = {
-- Generic properties ( all required)
    -- Graphic infos
    path     = "Game/assets/sprites/orc/Orc.png",
    action = "Idle",
    mirroredH = false, mirroredV = false,
    visible  = true,
    animations = {
        Idle = { row = 0, frameCount = 6, fps = 8, loop = true  },
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
    -- Physics properties
    maxSpeed = 3,
    speed = 3,
    friction = 30,
    jumpStrength = 8,
    isGrounded = false,

    -- Tracking properties
    playerId = -1,
    playerX = 0,
    playerY = 0,
    moving = false,
}

orc.__index = orc

function orc:new(overrides)
	local instance = {}
    setmetatable(instance, orc)
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

function orc:OnInit()
	self.Play(self.action)
    self.playerId = FindEntitybyName("player1")
end

function orc:OnUpdate(dt)
    local newAction = "Idle"

    -- Movements
    if IsEntityAlive(self.playerId) then
        local playerAction = GetEntityString(self.playerId, "action")
        local playerX = GetEntityFloat(self.playerId, "x")
        local playerY = GetEntityFloat(self.playerId, "y")

        if playerAction ~= "dead" then
            self.moving = true
            if playerX < self.x then
                self.dx = self.dx - self.speed * dt
            elseif playerX > self.x then
                self.dx = self.dx + self.speed * dt
            end
            if (playerY > self.y) and self.isGrounded then
                self.dy = self.dy - self.jumpStrength
            end
        else
            self.moving = false
        end
    end
    if self.moving == false then
        if self.dx > 0 then
            self.dx = math.max(0, self.dx - self.friction * dt)
        elseif self.dx < 0 then
            self.dx = math.min(0, self.dx + self.friction * dt)
        end
    end

    self.dx = math.max(-self.maxSpeed, math.min(self.dx, self.maxSpeed))

    self.dy = self.dy + 30 * dt

    self.x = self.x + self.dx
    self.y = self.y + self.dy
    
    self.isGrounded = false

    -- Animations
    if self.dx ~= 0 then
        newAction = "Walk"
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

function orc:fullCollision(overlapX, overlapY, overlapW, overlapH)
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

function orc:partialCollision(overlapX, overlapY, overlapW, overlapH)
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

function orc:OnCollision(tag, entityId, overlapX, overlapY, overlapW, overlapH)
    if tag == "wall" then
        self:fullCollision(overlapX, overlapY, overlapW, overlapH)
    elseif tag == "player" then
        self:partialCollision(overlapX, overlapY, overlapW, overlapH)
    elseif tag == "orc" then
        self:partialCollision(overlapX, overlapY, overlapW, overlapH)
    end
end

function orc:OnDestroy()
	--Empty
end