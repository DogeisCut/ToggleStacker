#include "stdafx.h"
#include "ToggleStacker.h"
#include <Spore/App/cPropManager.h>
#include <Spore\Properties.h>

using namespace UTFWin;
bool togglestacker_active = false;

//saved daa
int partcount = 0;

ToggleStacker::ToggleStacker()
{
	App::AddUpdateFunction(this);
}


ToggleStacker::~ToggleStacker()
{
}

void ToggleStacker::Update()
{
	if (!Editor.IsMode(Editors::Mode::BuildMode)/* || !togglestacker_active*/) return;

	if (Editor.GetEditorModel() == nullptr) return;

	//if (partcount != Editor.GetEditorModel()->mRigblocks.size()) {
		for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
		{
			part->mModelTypesNotToInteractWith.clear();
			part->mModelTypesToInteractWith.clear();
			if (!togglestacker_active) {
				PropertyListPtr propList = part->mpPropList;
				vector<ResourceKey> notInteractNames;
				App::Property::GetArrayKey(propList.get(), 0x044DCD0A, notInteractNames);
				for each (ResourceKey name in notInteractNames) {
					part->mModelTypesNotToInteractWith.push_back(name);
				}
				vector<ResourceKey> interactNames;
				App::Property::GetArrayKey(propList.get(), 0x6FFDED92, interactNames);
				for each (ResourceKey name in interactNames) {
					part->mModelTypesToInteractWith.push_back(name);
				}
			}
		}
		partcount = Editor.GetEditorModel()->mRigblocks.size();
	//}
}


void ToggleStacker::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	togglestacker_active = !togglestacker_active;
	if (togglestacker_active) {
		App::ConsolePrintF("ToggleStacker activated");
		
		/*
		for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
		{
			part->mModelTypesNotToInteractWith.clear();
			part->mModelTypesToInteractWith.clear();
		}
		partcount = Editor.GetEditorModel()->mRigblocks.size();
		*/
	} else {
		App::ConsolePrintF("ToggleStacker deactivated");

		/*
		if (Editor.IsMode(Editors::Mode::BuildMode) || togglestacker_active) {
			if (Editor.GetEditorModel() != nullptr) {
				for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
				{
					PropertyListPtr propList = part->mpPropList;
					vector<ResourceKey> notInteractNames;
					App::Property::GetArrayKey(propList.get(), 0x044DCD0A, notInteractNames);
					for each (ResourceKey name in notInteractNames) {
						part->mModelTypesNotToInteractWith.push_back(name);
					}
					vector<ResourceKey> interactNames;
					App::Property::GetArrayKey(propList.get(), 0x6FFDED92, interactNames);
					for each (ResourceKey name in interactNames) {
						part->mModelTypesToInteractWith.push_back(name);
					}
				}
			}
		}
		*/
	}
}

const char* ToggleStacker::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Toggles the ability to stack any part on each other.";
	}
	else {
		return "ToggleStacker: Toggles the ability to stack any part on each other.";
	}
}

int ToggleStacker::AddRef()
{
	return DefaultRefCounted::AddRef();
}

int ToggleStacker::Release()
{
	return DefaultRefCounted::Release();
}