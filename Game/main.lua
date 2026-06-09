gravity = 30
ResolutionX = 1920
ResolutionY = 1080

CreateWindow("TIT4N",  ResolutionX, ResolutionY)
SetFrameRate(60)
SetWindowFullscreen(true)
LoadScene("menus/menu1")
forest = CreateSound("assets/sounds/forest.wav")
PlaySound(forest)
LoopSound(forest, true)
music = CreateSound("assets/sounds/music.wav")
PlaySound(music)
LoopSound(music, true)

function OnUpdate(dt)
	if GetKeyState("Escape") == 2 then
		Quit()
	end
end