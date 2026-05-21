button = {
    -- Graphic infos
    path     = "Game/assets/sprites/button/button.png",
    action = "default",
    mirroredH = false, mirroredV = false,
    visible  = true,
    animations = {
        default = { row = 0, frameCount = 1, fps = 0, loop = false  },
        clicked = { row = 1, frameCount = 1, fps = 0, loop = false }
    },

    -- Position infos
    x = 0, y = 0, z = 0,
    angle = 0,
    srcrect  = { x = 0, y = 0, w = 590, h = 260 },
    dstScale = 1,

    -- Behavior infos
    hitbox  = { ox = 0, oy = 0, w = 0, h = 0 },
    wasClicked = false
}

button.__index = button

function button:new(overrides)
	local instance = {}
    setmetatable(instance, button)
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

function button:OnInit()
    self.Play(self.action)
end

function button:OnUpdate(dt)
    local newAction = "default"

    local mouseOver = false

    local mouseX = GetMouseState("x")
    local mouseY = GetMouseState("y")
    local mouseLeft = GetMouseState("leftButton")

    if mouseX >= self.x and mouseX <= self.x + self.srcrect.w * self.dstScale and
       mouseY >= self.y and mouseY <= self.y + self.srcrect.h * self.dstScale then
       mouseOver = true
    end

    if (mouseLeft == 2 or mouseLeft == 1) and mouseOver and self.visible then
        self.wasClicked = true
        newAction = "clicked"
    end

    if mouseLeft == 0 and mouseOver and self.wasClicked then
        self:OnClick()
        self.wasClicked = false
    end

    if newAction ~= self.action then
        self.action = newAction
        self.Play(self.action)
    end
end

function button:OnCollision(tag, entityId, overlapX, overlapY, overlapW, overlapH)
    -- Empty
end

function button:OnClick()
    -- Empty
end

function button:OnDestroy()
	--Empty
end
