#pragma once

bool showFbxConverter = false;
std::string fbxFile = "";
std::string exportId = "";
std::string exportDesc = "";
AssimpConverter* fbxConverter = nullptr;

void BoolEditor(Engine::Scripting::Attribute^ attrib)
{
	bool tmp = (bool)attrib->getValue();

	bool value = (bool)tmp;

	if (ImGui::Checkbox(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value))
	{
		attrib->setValue(value, false);
		attrib->setType(bool::typeid);
	}
}

void StringEditor(Engine::Scripting::Attribute^ attrib)
{
	String^ value = (String^)attrib->getValue();
	int valueLen = value->Length;
	char* data = new char[valueLen + 1 * 8];

	strcpy(data, CastStringToNative(value).c_str());

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 25);
	if (ImGui::InputText(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), data, valueLen + 1 * 8))
	{
		attrib->setValue(gcnew String(data));
	}

	delete[] data;
}

void ColorEditor(Engine::Scripting::Attribute^ attrib) 
{
	Engine::Components::Color^ value = nullptr;

	if (attrib->getValue()->GetType() == Newtonsoft::Json::Linq::JObject::typeid)
	{
		auto v = (Newtonsoft::Json::Linq::JObject^)attrib->getValue();
		value = v->ToObject<Engine::Components::Color^>();
	}
	else
	{
		value = (Engine::Components::Color^)attrib->getValue();
	}


	auto float4 = ImGui::ColorConvertU32ToFloat4(ImU32(value->toHex()));

	float rawData[4] =
	{
		float4.x,
		float4.y,
		float4.z,
		float4.w
	};

	if (ImGui::ColorEdit4(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), rawData))
	{
		attrib->setValue(gcnew Engine::Components::Color(ImGui::ColorConvertFloat4ToU32(ImVec4(rawData[0], rawData[1], rawData[2], rawData[3]))), false);
	}
}

void IntegerEditor(Engine::Scripting::Attribute^ attrib) 
{
	int value = (int)attrib->getValue();

	if (ImGui::InputInt(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 1, 1))
	{
		attrib->setValue(gcnew Int32(value), false);
		attrib->setType(Int32::typeid);
	}
}

void EnumEditor(Engine::Scripting::Attribute^ attrib)
{
	System::Enum^ enumerator = attrib->getValue<System::Enum^>();
	auto underlyingType = enumerator->GetType();
	auto enumNames = enumerator->GetNames(underlyingType);

	if (ImGui::BeginCombo(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), CastStringToNative(enumerator->ToString()).c_str()))
	{
		for (int x = 0; x < enumNames->Length; x++)
		{
			auto name = enumNames[x];
			bool isSame = (bool)name->Equals(enumerator->ToString());
			if (ImGui::Selectable(CastStringToNative(name).c_str(), &isSame))
			{
				attrib->setValue(enumerator->ToObject(attrib->getCurrentType(), x), false);
			}
		}

		ImGui::EndCombo();
	}
}


void EnableFBXConverter(std::string fbxFilePath)
{
	showFbxConverter = true;
	fbxFile = fbxFilePath;
}

void OnFBXConverted(String^ fileName)
{
	fbxConverter = new AssimpConverter(fbxFile, CastStringToNative(fileName), exportId);

	delete fbxConverter;
}

void RenderFBXConverter() 
{
	if(showFbxConverter)
		ImGui::OpenPopup("FBX Converter");

	bool conversion = false;

	if (ImGui::BeginPopupModal("FBX Converter", &showFbxConverter))
	{
		ImGui::Text("Export Format");

		if (ImGui::BeginCombo("###EXPORT_FORMAT", exportDesc.c_str()))
		{
			for (int x = 0; x < getAssimpExporters(); x++)
			{
				if (ImGui::Selectable(getAssimpExporterDescription(x)))
				{
					exportDesc = std::string(getAssimpExporterDescription(x));
					exportId = std::string(getAssimpExporterId(x));
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button("Convert"))
		{
			conversion = true;
			showFbxConverter = false;
		}

		ImGui::EndPopup();
	}

	if (conversion)
	{
		auto fileExplorer = Singleton<Engine::Editor::Gui::fileExplorer^>::Instance;

		fileExplorer->SetWindowName("Convert FBX File");
		fileExplorer->setExplorerMode(Engine::Editor::Gui::explorerMode::Save);
		fileExplorer->Open();

		fileExplorer->OnCompleted(gcnew Engine::Editor::Gui::onFileSelected(&OnFBXConverted));
	}
}