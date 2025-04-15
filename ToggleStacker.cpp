#include "stdafx.h"
#include "ToggleStacker.h"
#include <Spore/App/cPropManager.h>
#include <Spore\Properties.h>

using namespace UTFWin;

enum class Mode {
	DEFAULT,
	STACK_ALL,
	STACK_NONE,
	CUSTOM
};

Mode currentMode = Mode::DEFAULT;

ToggleStacker::ToggleStacker()
{
	App::AddUpdateFunction(this);
}

ToggleStacker::~ToggleStacker()
{
}

void ToggleStacker::Update()
{
	if (!Editor.IsMode(Editors::Mode::BuildMode)) return;

	if (Editor.GetEditorModel() == nullptr) return;

	for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
	{
		part->mModelTypesNotToInteractWith.clear();
		part->mModelTypesToInteractWith.clear();
		if (currentMode == Mode::DEFAULT) {
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


void ToggleStacker::ParseLine(const ArgScript::Line& line)
{
	if (currentMode == Mode::DEFAULT)
	{
		currentMode = Mode::STACK_ALL;
		App::ConsolePrintF("ToggleStacker activated, all parts will now stack.");

	} 
	else 
	{
		currentMode = Mode::DEFAULT;
		App::ConsolePrintF("ToggleStacker deactivated, default behavior restored.");
	}
}

const char* ToggleStacker::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Toggles the ability to stack any part on each other.";
	}
	else 
	{
		return "ToggleStacker: Toggles the ability to stack any part on each other, or disable stacking alltogether, on specific parts or all of them.";
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