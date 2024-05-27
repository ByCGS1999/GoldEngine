function Start()
	attributes.removeAttribute("Test Attribute");
	attributes.setAttribute(Attribute.create("Test Attribute", "Hello world"));

	Logging.Log(attributes.getAttribute("Test Attribute").getValue());
end


