CreateWindow("TIT4N",  1920, 1080)
SetFrameRate(60)
SetWindowFullscreen(true)
LoadScene("menus/menu1")

-- Constant
gravity = 30



function OnUpdate(dt)
	if GetKeyState("Escape") == 2 then
		Quit()
	end
end