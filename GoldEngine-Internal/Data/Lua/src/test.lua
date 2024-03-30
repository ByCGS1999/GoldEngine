function Start()
	Logging.LogError("Called logerror from lua file.");
	local newAttribute = Attribute.create("Value", "fumada?");
	attributes.setAttribute("fumada34", 1);

	attributes.addAttribute(newAttribute);
end

function Draw()
	local value = attributes.getAttribute("fumada34");
	script.GetTransform().Position.y = value;
end
