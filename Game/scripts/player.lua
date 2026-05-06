player = {
	-- Generic properties
    path     = "Game/assets/sprites/player/Soldier.png",
    x = 0, y = 0, z = 0,
    srcrect  = { x = 0, y = 0, w = 100, h = 100 },
    dstScale = 5,
    visible  = true,
    rotation = 0,
    tags	 = { "player", "character" },
    hitboxs  = { { ox = 0, oy = 0, w = 100, h = 100 } },
    paused = false,
    animations = {
        Idle = { row = 0, frameCount = 6, fps = 9, loop = true  },
        Walk = { row = 1, frameCount = 8, fps = 8, loop = true  },
        Attack = { row = 2, frameCount = 6, fps = 12, loop = false },
    },

    -- Custom properties
    hp    = 100,
    speed = 75
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
	--Empty
end

function player:OnUpdate(dt)
    local action = "Idle"
	local dx = 0
    local dy = 0
    local temp = 100

    if GetKeyState("W") == 2 or GetKeyState("W") == 1 then
        dy = dy - self.speed * dt
        action = "Walk"
    end
    if GetKeyState("S") == 2 or GetKeyState("S") == 1 then
        dy = dy + self.speed * dt
        action = "Walk"
    end
    if GetKeyState("A") == 2 or GetKeyState("A") == 1 then
        dx = dx - self.speed * dt
        action = "Walk"
    end
    if GetKeyState("D") == 2 or GetKeyState("D") == 1 then
        dx = dx + self.speed * dt
        action = "Walk"
    end
    self.x = self.x + dx
    self.y = self.y + dy

    temp = temp - dt
    if temp <= 0 then 
        self.srcrect.x = self.srcrect.x + 100
        temp = temp + 100
    end

    if action == "Walk" then
        self.srcrect.y = 100
        if self.srcrect.x >= 500 then
            self.srcrect.x = 0
        end
    else
        self.srcrect.y = 0
        if self.srcrect.x >= 700 then
            self.srcrect.x = 0
        end
    end
end

function player:OnDestroy()
	--Empty
end
