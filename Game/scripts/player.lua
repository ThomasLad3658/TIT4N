player = {
-- Generic properties ( all required)
    -- Graphic infos
    path     = "assets/sprites/player/Soldier.png", -- Path for the texture file
    action = "Idle", -- Current action of the entity
    mirroredH = false, mirroredV = false, -- Horizontal and vertical texture mirroring
    visible  = true, -- If the texture is presented on the screen
    animations = { -- Animations details
        Idle = { row = 0, frameCount = 6, fps = 8, loop = true  },
        Walk = { row = 1, frameCount = 8, fps = 8, loop = true  },
        Attack = { row = 2, frameCount = 6, fps = 12, loop = false },
        dead = { row = 6, frameCount = 4, fps = 4, loop = false }
    },

    -- Position infos
    x = 0, y = 0, z = 0,
    dx = 0, dy = 0,
    angle = 0, -- rotation angle of the texture
    srcrect  = { x = 0, y = 0, w = 100, h = 100 }, -- Rectangle on the source texture (PNG file)
    dstScale = 5, -- How many times bigger on the screen compared to the source rectangle

    -- Behavior infos
    hitbox  = { ox = 220, oy = 195, w = 60, h = 90 }, -- Rectangle for the collisions, with an offset (ox, oy) from the entity position (x, y)

-- Custom properties (optional properties) do what you want here
    hp = 100, -- Health points of the player
    maxSpeed = 8, -- Maximum horizontal speed
    speed = 20, -- Acceleration speed
    friction = 30, -- Friction applied when no input is given
    jumpStrength = 12, -- Initial jump velocity
    isGrounded = false, -- If the player is on the ground
    timeBeforeSuicide = 1, -- Time before the player's death animations is finished (in seconds)
}

player.__index = player

-- Don't touch this
function player:new(overrides) -- It applies the original table's values over the script's values when a new table based on this one is created
	local instance = {}
    setmetatable(instance, player)
    if overrides then
        for key, value in pairs(overrides) do
            instance[key] = value
        end
    end
    return instance
end

function player:OnInit()
	self.Play(self.action) -- Start the default animation (idle)
end

function player:OnUpdate(dt)
    local newAction -- Futur action (applied at the end of the function)

    -- Movements
    local KeyA = GetKeyState("A") -- Get the state of the keys and save the values to save some performance (instead of calling GetKeyState multiple times for the same key)
    local KeyB = GetKeyState("D")
    local KeySpace = GetKeyState("Space")

    if KeyA == 2 or KeyA == 1 then
        self.dx = self.dx - self.speed * dt -- Apply acceleration to the horizontal speed (dx) based on the speed property and the time since the last frame (dt)
    end
    if KeyB == 2 or KeyB == 1 then
        self.dx = self.dx + self.speed * dt -- Same for the right key
    end
    if KeySpace == 2 and self.isGrounded then -- If the jump key is pressed and the player is on the ground, apply the vertical jump velocity (negative because the y axis is inverted in the engine)
        self.dy = self.dy - self.jumpStrength
    end

    if KeyA == 0 and KeyB == 0 then -- If no horizontal keys are pressed, apply friction to gradually stop the player
        if self.dx > 0 then
            self.dx = math.max(0, self.dx - self.friction * dt)
        elseif self.dx < 0 then
            self.dx = math.min(0, self.dx + self.friction * dt)
        end
    end

    self.dx = math.max(-self.maxSpeed, math.min(self.dx, self.maxSpeed)) -- Limit the horizontal speed to the maximum speed (both positive and negative)

    if self.hp <= 0 then -- If the player is dead, stop all movements
        self.dx = 0
        self.dy = 0
    end

    self.dy = self.dy + gravity * dt -- Apply gravity to the vertical speed (dy)

    self.x = self.x + self.dx -- Apply the horizontal and vertical speeds to the position
    self.y = self.y + self.dy
    
    self.isGrounded = false -- Reset the grounded state, it will be set to true again if a collision with the ground is detected in the collision function

    -- Check life
    if self.hp <= 0 then
        if self.timeBeforeSuicide <= 0 then -- If the death timer is finished, remove the player from the scene
            self.Suicide()
        else
            self.timeBeforeSuicide = self.timeBeforeSuicide - dt -- If the player is dead, start the death timer
        end
    end

    -- Animations
    if self.dx ~= 0 then -- If the player is moving horizontally, set the action to "Walk"
        newAction = "Walk"
    else
        newAction = "Idle" -- Else, the action is "Idle"
    end
    if self.hp <= 0 then -- If the player is dead, set the action to "dead"
        newAction = "dead"
    end
    if self.dx < 0 then -- Mirror the texture in the direction where the player is facing
        self.mirroredH = true
    elseif self.dx > 0 then
        self.mirroredH = false
    end
    if newAction ~= self.action then -- If the newAction is different from the current action, update the action and play the new action
        self.action = newAction -- The newAction variable is just so that we don't call Play() if the action didn't change (performance issues)
        self.Play(self.action)
    end
end

function player:fullCollision(overlapX, overlapY, overlapW, overlapH)
    if overlapW < overlapH and overlapH > gravity then -- If the overlap width is smaller than the overlap height, it's a horizontal collision, else it's a vertical collision
        if overlapX + overlapW/2 > self.x + self.hitbox.ox + self.hitbox.w/2 then -- If the center of the overlap is on the right side of the player's hitbox, it's a collision on the right side, else it's a collision on the left side
            self.x = self.x - overlapW -- Move the player out of the collision
            self.dx = math.min(self.dx, 0) -- Set the moving speed to 0
        else
            self.x = self.x + overlapW -- Same for the left side collision
            self.dx = math.max(self.dx, 0)
        end
    else
        if overlapY + overlapH/2 > self.y + self.hitbox.oy + self.hitbox.h/2 then -- Same for the vertical collisions
            self.y = self.y - overlapH
            self.isGrounded = true -- If the player is colliding with something under him, he's on the ground
            self.dy = math.min(self.dy, 0)
        else
            self.y = self.y + overlapH
            self.dy = math.max(self.dy, 0)
        end
    end
end

function player:partialCollision(overlapX, overlapY, overlapW, overlapH) -- Same as the fullCollision, but half the correction if the other also has a partial collision function (on the horizontal axis)
    if overlapW < overlapH then
        if overlapX + overlapW/2 > self.x + self.hitbox.ox + self.hitbox.w/2 then
            self.x = self.x - overlapW / 2
            self.dx = math.min(self.dx, 0)
        else
            self.x = self.x + overlapW / 2
            self.dx = math.max(self.dx, 0)
        end
    else
        if overlapY + overlapH/2 > self.y + self.hitbox.oy + self.hitbox.h/2 then -- We only correct the player fully on the vertical axis to avoid collision bug with gravity
            self.y = self.y - overlapH
            self.isGrounded = true
            self.dy = math.min(self.dy, 0)
        end
    end
end

function player:OnCollision(tag, entityId, overlapX, overlapY, overlapW, overlapH) -- calls the right effect depending on the tag of the other entity
    if tag == "wall" then
        self:fullCollision(overlapX, overlapY, overlapW, overlapH)
    elseif tag == "player" then
        self:partialCollision(overlapX, overlapY, overlapW, overlapH)
    elseif tag == "orc" then
        self:partialCollision(overlapX, overlapY, overlapW, overlapH)
    elseif tag == "orc_attack" then -- We would apply knockback here if we wanted some
        self.hp = self.hp - 10
    end
end

function player:OnDestroy() -- Does nothing when the player is removed from the scene
	--Empty
end

--[[
    In general, I want to call the engine's functions as less as possible for performance reasons    

    -- Required   -> They can cause a crash if not specified
    tag           -> Required
    x, y          -> Required
    hitbox        -> Required
    visible       -> Can be nil if path = "" (will be set to false)
    srcrect       -> Can be nil if path = "" or visible = false (will be set to 0)
    dstScale      -> Can be nil if path = "" or visible = false (will be set to 0)
    angle         -> Can be nil if path = "" or visible = false (will be set to 0)
    z             -> Can be nil if path = "" or visible = false (will be set to 0)
    mirroredH     -> Can be nil if path = "" or visible = false (will be set to false)
    mirroredV     -> Can be nil if path = "" or visible = false (will be set to false)
    animations    -> Crash if Play() is called without this table
    new()         -> Never modify, never remove, don't touch it

    -- Optional   -> Used by the engine if specified, but won't cause a crash if not specified
    path          -> Will be set to "" if not specified, and the entity will be invisible
    name		  -> Will be set to "" if not specified
    OnInit()      -> Will be skipped if not specified
    OnUpdate()    -> Will be skipped if not specified
    OnCollision() -> Will be skipped if not specified
    OnDestroy()   -> Will be skipped if not specified

    -- Custom     -> Custom properties and functions for your entities, they will be ignored by the engine
    action        -> to help you manage your animations
    dx, dy        -> to help you manage your movements
    Anything      -> More useful functions and properties can be added, as much as you want
]]