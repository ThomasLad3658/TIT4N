update = {
	-- Generic properties
    path     = "Game/assets/sprites/player/Soldier.png",
    x = 0, y = 0, z = 0,
    angle = 0,
    srcrect  = { x = 0, y = 0, w = 0, h = 0 },
    dstScale = 5,
    tags	 = { "update" },
    hitboxs  = { { ox = 0, oy = 0, w = 0, h = 0 } },
    paused = false,
}

update.__index = update

function update:new(overrides)
	local instance = {}
    setmetatable(instance, update)
    if overrides then
        for key, value in pairs(overrides) do
            instance[key] = value
        end
    end
    return instance
end

function update:OnInit()
    --Le vide
end

function update:OnUpdate(dt)
    local sound
    if GetKeyState("C") == 2 then
        sound = CreateSound("C:\\Users\\levra\\source\\repos\\ThomasLad3658\\TIT4N\\Game\\assets\\sounds\\example_loop.wav")
    end
    PlaySound(sound)
end

function update:OnDestroy()
	--Néant
end
