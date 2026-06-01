orc = {
-- Generic properties ( all required)
    -- Graphic infos
    path     = "assets/sprites/orc/Orc.png",
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
    hitbox  = { ox = 220, oy = 210, w = 60, h = 75 },

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

    -- Sound properties
    soundId = -1,

    -- Attack properties
    timeBeforeAttackFrame = 0,
    timeBeforeNextAttack = 0,
    canAttack = true,
    axe = {
        orcId = -1,
        tag = "orc_attack",
        x = 0, y = 0,
        hitbox = { ox = 0, oy = 0, w = 75, h = 130 },
        damage = 10,
        OnInit = function(self)
            if GetEntityBool(self.orcId, "mirroredH") then
                self.x = GetEntityFloat(self.orcId, "x") + GetEntityFloat(self.orcId, "hitbox.ox") - self.hitbox.w
            else
                self.x = GetEntityFloat(self.orcId, "x") + GetEntityFloat(self.orcId, "hitbox.ox") + self.hitbox.w
            end
            self.y = GetEntityFloat(self.orcId, "y") + GetEntityFloat(self.orcId, "hitbox.oy")
        end,
        OnUpdate = function(self, dt)
            self.Suicide()
        end
    },
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
    return instance
end

function orc:OnInit()
	self.Play(self.action)
    self.playerId = GetEntityIdByName("player1")
    soundId = CreateSound("assets/sounds/axe.wav")
end

function orc:OnUpdate(dt)
    local newAction = "Idle"

    -- Movements
    if IsEntityAlive(self.playerId) then
        local playerAction = GetEntityString(self.playerId, "action")
        local playerX = GetEntityFloat(self.playerId, "x") + GetEntityFloat(self.playerId, "hitbox.ox")
        local playerW = GetEntityFloat(self.playerId, "hitbox.w")
        local playerY = GetEntityFloat(self.playerId, "y") + GetEntityFloat(self.playerId, "hitbox.oy")
        local orcX = self.x + self.hitbox.ox
        local orcY = self.y + self.hitbox.oy
        local playerCenter = playerX + playerW / 2
        local orcCenter = orcX + self.hitbox.w / 2
        local distanceX = math.abs(playerCenter - orcCenter)

        if playerAction ~= "dead" then
            self.moving = true
            if playerCenter < orcCenter then
                self.dx = self.dx - self.speed * dt
            elseif playerCenter > orcCenter then
                self.dx = self.dx + self.speed * dt
            end
            if playerY - orcY < -self.hitbox.h / 2 and self.isGrounded then
                self.dy = self.dy - self.jumpStrength
            end
            if distanceX <= self.axe.hitbox.w and self.action ~= "Attack" then
                self.timeBeforeAttackFrame = 0.25
                self.timeBeforeNextAttack = 0.5
                newAction = "Attack"
                self.canAttack = true
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

    if self.dx < 0 then
        self.mirroredH = true
    elseif self.dx > 0 then
        self.mirroredH = false
    end

    self.dy = self.dy + gravity * dt

    self.x = self.x + self.dx
    self.y = self.y + self.dy
    
    self.isGrounded = false

    -- Attack
    if self.action == "Attack" then
        self.timeBeforeAttackFrame = self.timeBeforeAttackFrame - dt
        self.timeBeforeNextAttack = self.timeBeforeNextAttack - dt
        if self.timeBeforeAttackFrame <= 0 and self.canAttack then
            self.axe.orcId = self.getSelfId()
            if self.mirroredH then
                self.axe.x = self.x - self.axe.hitbox.w
            else
                self.axe.x = self.x + self.hitbox.w
            end
            self.axe.y = self.y
            self.CreateEntityFromEntity("axe")
            PlaySoundFromTo(soundId, 100, 1200)
            self.canAttack = false
        end
    end

    -- Animations
    if self.action == "Attack" then
        newAction = "Attack"
    end
    if self.dx ~= 0 and newAction == "Idle" then
        newAction = "Walk"
    end
    if self.timeBeforeNextAttack <= 0 and self.action == "Attack" then
        newAction = "Idle"
    end
    if newAction ~= self.action then
        self.action = newAction
        self.Play(self.action)
    end
end

function orc:fullCollision(overlapX, overlapY, overlapW, overlapH)
    if overlapW < overlapH and overlapH > gravity then
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
            self.x = self.x - overlapW / 2
            self.dx = math.min(self.dx, 0)
        else
            self.x = self.x + overlapW / 2
            self.dx = math.max(self.dx, 0)
        end
    else
        if overlapY + overlapH/2 > self.y + self.hitbox.oy + self.hitbox.h/2 then
            self.y = self.y - overlapH
            self.isGrounded = true
            self.dy = math.min(self.dy, 0)
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
	DestroySound(soundId)
end