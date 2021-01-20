panel = mcLuaPanelParent
inst = mc.mcGetInstance()

local BluetoothMPG = {}

function BluetoothMPG.RunMPG()
	panel:SetSizeHints(wx.wxDefaultSize, wx.wxDefaultSize)
	panel:Layout()
	panel:Centre(wx.wxBOTH)

	mc.mcMpgSetAccel(inst, 11, 80)  
	mc.mcMpgSetRate(inst, 11, 100)
	mc.mcMpgSetCountsPerDetent(inst, 11, 4)

	local mpgPosReg = mc.mcRegGetHandle(inst, "gRegs0/MPGPosition")
	mpgRotation = 0

	panel:Connect(wx.wxID_ANY, wx.wxEVT_MOUSEWHEEL, function(event)
		mpgRotation = mpgRotation + event:GetWheelRotation()
		mc.mcRegSetValueLong(mpgPosReg, (mpgRotation / 120))
		mc.mcMpgMoveCounts(inst, 11, (event:GetWheelRotation() / 120))
		--event:Skip()
	end)

--[[
	panel:Connect(wx.wxID_ANY, wx.wxEVT_ENTER_WINDOW, function(event)
		local xPos = event:GetX()
		local yPos = event:GetY()
		mc.mcCntlSetLastError(inst, 'X:' .. tostring(xPos) .. ' Y:' .. tostring(yPos))
	end)
]]
	panel:Connect(wx.wxID_ANY, wx.wxEVT_KEY_DOWN, function(event)
		--mc.mcCntlSetLastError(inst, tostring(event:GetKeyCode()))
		local keyPressed = event:GetKeyCode()

		if (keyPressed >= 340 and keyPressed <= 345) then -- axis selector
			mc.mcMpgSetAxis(inst, 11, keyPressed - 340)
		end

		if (keyPressed >= 346 and keyPressed <= 348) then -- increment selector
			local incSelected = 348 - keyPressed
			local incVal = 0

			if incSelected == 2 then
				incVal = 0.001
			elseif incSelected == 1 then
				incVal = 0.01
			elseif incSelected == 0 then 
				incVal = 0.1
			end

			mc.mcMpgSetInc(inst, 11, incVal)
		end
		--event:Skip()
	end)
end

return BluetoothMPG

--[[
local inst = mc.mcGetInstance()
local profile = mc.mcProfileGetName(inst)
local path = mc.mcCntlGetMachDir(inst)

package.path = path .. "\\Profiles\\" .. profile .. "\\Modules\\?.lua;" .. path .. "\\Modules\\?.lua;"
BluetoothMPG = require "BluetoothMPG"

BluetoothMPG.RunMPG()
]]