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
        Attack = { row = 2, frameCount = 6, fps = 12, loop = false },
    },

    -- Position infos
    x = 0, y = 0, z = 0,
    dx = 0, dy = 0,
    angle = 0,
    srcrect  = { x = 0, y = 0, w = 100, h = 100 },
    dstScale = 5,
    rotation = 0,

    -- Behavior infos
    tags	 = { "player", "character" },
    hitboxs  = { { ox = 0, oy = 0, w = 100, h = 100 } },
    paused = false,

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
    return instance
end

function player:OnInit()
	self.Play(self.action)
end

function player:OnUpdate(dt)
    local newAction = "Idle"
    dx = 0
    dy = 0

    -- Movements
    if GetKeyState("W") == 2 or GetKeyState("W") == 1 then
        dy = dy - self.speed * dt
        newAction = "Walk"
    end
    if GetKeyState("S") == 2 or GetKeyState("S") == 1 then
        dy = dy + self.speed * dt
        newAction = "Walk"
    end
    if GetKeyState("A") == 2 or GetKeyState("A") == 1 then
        dx = dx - self.speed * dt
        newAction = "Walk"
    end
    if GetKeyState("D") == 2 or GetKeyState("D") == 1 then
        dx = dx + self.speed * dt
        newAction = "Walk"
    end
    self.x = self.x + dx
    self.y = self.y + dy
    
    -- Animations
    if dx < 0 then
        self.mirroredH = true
    else if dx > 0 then
            self.mirroredH = false
        end
    end
    if newAction ~= self.action then
        self.action = newAction
        self.Play(self.action)
    end
end

function OnCollision()
    --Empty
end

function player:OnDestroy()
	--Empty
end
