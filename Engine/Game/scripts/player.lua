player = {
	-- Generic properties
    path     = "Game/assets/sprites/player/Soldier.png",
    x = 0, y = 0, z = 0,
    srcrect  = { x = 0, y = 0, w = 100, h = 100 },
    dstScale = 5,
    visible  = true,
    rotation = 0,
    tags	 = { "player", "character" },
    paused = false,

    -- Custom properties
    hp    = 100,
    speed = 10,
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
	--Empty
end

function player:OnDestroy()
	--Empty
end
