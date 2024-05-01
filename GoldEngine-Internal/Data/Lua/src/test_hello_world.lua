local triggeredError = false;
local triggerCheck = true;


function getSkybox()
	return ObjectManager.GetFirstObjectOfName("Skybox");
end

function Update()
	local skybox = getSkybox();

	if(triggerCheck) then
		return;
	end

	if (skybox == nil) then
		return;
	end

	if(skybox ~= nil) then
		if(Input.IsKeyDown(65)) then
			skybox.transform.position.x = skybox.transform.position.x + 5;
		else if(Input.IsKeyDown(68)) then
			skybox.transform.position.x = skybox.transform.position.x - 5;
		end
		end
	else
		if(not triggeredError) then
			Logging.LogError("Skybox not set to an instance.");
			triggeredError = true;
		end
	end
end

