
#include "../imgui.h"

namespace Engine::Editor::Gui
{
	enum class explorerMode
	{
		Open,
		Save
	};

	ref class fileExplorer
	{
	private:
		std::string* title;
		List<String^>^ extensions = gcnew List<String^>(0);
		bool* open;
		
		List<String^>^ files;
		String^ currentRoute;
		String^ selectedFile;
		explorerMode mode;
		String^ dialogResult;
		Action<String^>^ bind;

	public:
		fileExplorer(std::string* title) : fileExplorer(title, gcnew List<String^>())
		{

		}

		fileExplorer(std::string* title, List<String^>^ ext)
		{
			this->title = title;
			this->extensions = ext;
			open = false;
			files = gcnew List<String^>();
			currentRoute = Directory::GetCurrentDirectory() + "/Data";
			dialogResult = "";
		}

	public:
		void setExplorerMode(explorerMode mode)
		{
			this->mode = mode;
		}

		~fileExplorer()
		{
			delete title;
			delete open;
		}

	public:
		void Open()
		{
			dialogResult = "";
			open = (bool*)true;
		}

		void Close()
		{
			open = (bool*)false;
		}
		
	public:
		void SetWindowName(std::string* name)
		{
			delete title;
			this->title = name;
		}

	private:
		void fetchFiles(String^ route)
		{
			files->Clear();

			if (!Directory::Exists(route)) return;

			for each (String ^ s in Directory::GetFileSystemEntries(route))
			{
				if (extensions->Count <= 0)
				{
					files->Add(s);
				}
				else
				{
					for each (String^ extension in extensions)
					{
						if (s->Contains(extension))
						{
							files->Add(s);
						}
					}
				}
			}
		}

	private:
		bool isDirectory(String^ path)
		{
			if (File::Exists(path))
				return false;

			return true;
		}

	public:
		void OnCompleted(Action<String^>^ t) {
			bind = t;
			if (dialogResult != "")
			{
				t->Invoke(dialogResult);
				dialogResult = "";
				bind = nullptr;
			}
		}

		public:
			void TryComplete()
			{
				if(bind != nullptr)
					OnCompleted(bind);
			}

	public:
		String^ getDialogResult()
		{
			return dialogResult;
		}

	public:
		void DrawExplorer()
		{
			if (!open)
				return;

			ImGui::OpenPopup(title->c_str());
			bool isFile = !isDirectory(selectedFile);
			ImGui::BeginPopupModal(title->c_str(), NULL, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize);

			ImGui::SetWindowSize({ 899,462 });

			char* route = new char[currentRoute->Length + 8];
			strcpy(route, CastStringToNative(currentRoute).c_str());

			if (ImGui::Button("<"))
			{
				if (!currentRoute->Contains("/"))
				{
					currentRoute = currentRoute->Substring(
						0, currentRoute->LastIndexOf("\\"));
				}
				else
				{
					currentRoute = currentRoute->Substring(
						0, currentRoute->LastIndexOf("/"));
				}
			}
			ImGui::SameLine();
			ImGui::Text("Path: ");
			ImGui::SameLine();
			if (ImGui::InputText("###PATH", route, currentRoute->Length+8, ImGuiInputTextFlags_None | ImGuiInputTextFlags_CallbackCompletion))
			{
				currentRoute = gcnew String(route);
			}

			ImGui::SameLine();

			ImGui::BeginListBox("###FILES", {
				ImGui::GetWindowSize().x-25,
				ImGui::GetWindowSize().y-82
				});

			fetchFiles(currentRoute);

			for each (String^ fileName in files)
			{
				if (isDirectory(fileName)) {
					if (ImGui::Selectable(CastStringToNative("[D] " + fileName).c_str()))
					{
						selectedFile = fileName;
					}
				}
				else
				{
					if (ImGui::Selectable(CastStringToNative("[F] " + fileName).c_str()))
					{
						selectedFile = fileName;
					}
				}
			}

			ImGui::EndListBox();

			if (mode == explorerMode::Open || !isFile)
			{
				if (ImGui::Button("Open"))
				{
					if (!isFile)
					{
						currentRoute = selectedFile;
					}
					else
					{
						dialogResult = selectedFile;
						Close();
						ImGui::CloseCurrentPopup();
					}
				}
			}
			else
			{
				if (ImGui::Button("Save"))
				{
					dialogResult = selectedFile;
					Close();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				Close();
				ImGui::CloseCurrentPopup();
			}


			ImGui::EndPopup();
		}
	};
}