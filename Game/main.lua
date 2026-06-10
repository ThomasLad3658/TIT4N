-- Constant
gravity = 30

CreateWindow("TIT4N",  1920, 1080)
SetFrameRate(60)
SetWindowFullscreen(true)
LoadScene("menus/menu1")
--[[
forest = CreateSound("assets/sounds/forest.wav")
PlaySound(forest)
LoopSound(forest, true)
music = CreateSound("assets/sounds/music.wav")
PlaySound(music)
LoopSound(music, true)
]]

function OnUpdate(dt)
	if GetKeyState("Escape") == 2 then
		Quit()
	end
end