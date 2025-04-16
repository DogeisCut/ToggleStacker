#include "stdafx.h"
#include "ToggleStacker.h"
#include <Spore/App/cPropManager.h>
#include <Spore\Properties.h>
#include <unordered_map>

using namespace UTFWin;

enum class Mode {
	DEFAULT,
	STACK_ALL,
	STACK_NONE,
	STACK_NO_BASE,
};

Mode currentMode = Mode::DEFAULT;

struct CustomModeEntry {
	EditorRigblockPtr part;
	Mode mode;
};

vector<ToggleStacker::CustomModeEntry> customModeParts;

ToggleStacker::ToggleStacker()
{
	App::AddUpdateFunction(this);
}

ToggleStacker::~ToggleStacker()
{
}

void ToggleStacker::Update()
{
	static const vector<ResourceKey> notInteractKeys = {
		// Creature parts
		{id("nose"), 0x0, id("creature")},
		{id("eye"), 0x0, id("creature")},
		{id("antenna"), 0x0, id("creature")},
		{id("ear"), 0x0, id("creature")},
		{id("detail"), 0x0, id("creature")},
		{id("fin"), 0x0, id("creature")},
		{id("wing"), 0x0, id("creature")},
		{id("weapon"), 0x0, id("creature")},
		{id("foot"), 0x0, id("creature")},
		{id("grasper"), 0x0, id("creature")},
		{id("mouth"), 0x0, id("creature")},

		// Flora parts
		{id("cap"), 0x0, id("plant")},
		{id("fruit"), 0x0, id("plant")},
		{id("leaf"), 0x0, id("plant")},

		// Outfitter parts
		{id("shoulderpad"), 0x0, id("tribal")},
		{id("hat"), 0x0, id("tribal")},
		{id("mask"), 0x0, id("tribal")},
		{id("collar"), 0x0, id("tribal")},
		{id("feather"), 0x0, id("tribal")},
		{id("horn"), 0x0, id("tribal")},
		{id("symbol"), 0x0, id("tribal")},

		// Vehicle parts
		{id("wing"), 0x0, id("vehicle")},
		{id("window"), 0x0, id("vehicle")},
		{id("wheel"), 0x0, id("vehicle")},
		{id("weapon"), 0x0, id("vehicle")},
		{id("ski"), 0x0, id("vehicle")},
		{id("sail"), 0x0, id("vehicle")},
		{id("ringpop"), 0x0, id("vehicle")},
		{id("propeller"), 0x0, id("vehicle")},
		{id("paddle"), 0x0, id("vehicle")},
		{id("medallion"), 0x0, id("vehicle")},
		{id("light"), 0x0, id("vehicle")},
		{id("jetengine"), 0x0, id("vehicle")},
		{id("flag"), 0x0, id("vehicle")},
		{id("detail"), 0x0, id("vehicle")},
		{id("Cultural"), 0x0, id("vehicle")},
		{id("balloon"), 0x0, id("vehicle")},

		// Building parts
		{id("window"), 0x0, id("building")},
		{id("detail"), 0x0, id("building")},
		{id("stair"), 0x0, id("building")},
		{id("floor"), 0x0, id("building")},
		{id("door"), 0x0, id("building")},
		{id("chimney"), 0x0, id("building")},
		{id("balcony"), 0x0, id("building")}
	};

	if (!Editor.IsMode(Editors::Mode::BuildMode) || Editor.GetEditorModel() == nullptr) {
		customModeParts.clear();
		return;
	}

	for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
	{
		part->mModelTypesNotToInteractWith.clear();
		part->mModelTypesToInteractWith.clear();

		switch (currentMode)
		{
			case Mode::DEFAULT:
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
					break;
				}
			case Mode::STACK_ALL:
				break;
			case Mode::STACK_NONE:
				{
					for (const ResourceKey& key : notInteractKeys)
					{
						part->mModelTypesNotToInteractWith.push_back(key);
					}
					break;
				}
			case Mode::CUSTOM:
				break;
			default:
				break;
		}
	}

	for each (ToggleStacker::CustomModeEntry entry in customModeParts)
	{
		EditorRigblockPtr part = entry.part;
		Mode mode = entry.mode;

		part->mModelTypesNotToInteractWith.clear();
		part->mModelTypesToInteractWith.clear();

		switch (entry.mode) {
			case Mode::DEFAULT:
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
				break;
			}
			case Mode::STACK_ALL:
				break;
			case Mode::STACK_NONE:
			{
				for (const ResourceKey& key : notInteractKeys)
				{
					part->mModelTypesNotToInteractWith.push_back(key);
				}
				break;
			}
			case Mode::CUSTOM:
				break;
			default:
				break;
		}
	}
}


void ToggleStacker::ParseLine(const ArgScript::Line& line)
{

	auto mode = line.GetOption("mode", 1);
	auto custom = line.GetOption("custom", 1);

	if (mode != nullptr) {
		std::string m = mode[0];
		if (m == "all" || m == "stack" || m == "yes") {
			currentMode = Mode::STACK_ALL;
			App::ConsolePrintF("ToggleStacker activated (STACK_ALL), all parts will now stack.");
		}
		else if (m == "none" || m == "unstack" || m == "no") {
			currentMode = Mode::STACK_NONE;
			App::ConsolePrintF("ToggleStacker activated (STACK_NONE), no parts will stack.");
		}
		else if (m == "default" || m == "deactivate" || m == "off" || m == "false") {
			currentMode = Mode::DEFAULT;
			App::ConsolePrintF("ToggleStacker deactivated (DEFAULT), default behavior restored.");
		}
		else {
			App::ConsolePrintF("Unknown Togglestacker mode. Avaliable modes: 'all / stack / yes', 'none / unstack / no', 'default / deactivate / off / false'.");
		}
	}

	if (custom != nullptr) {
		if (!Editor.IsMode(Editors::Mode::BuildMode) || Editor.GetEditorModel() == nullptr) {
			App::ConsolePrintF("Use this option in the editor while hovering or selecting a part!");
			return;
		}
		std::string c = custom[0];
		if (c == "add" || c == "include") {
			EditorRigblockPtr targetPart = Editor.mpSelectedPart ? Editor.mpSelectedPart : Editor.mpActivePart;
			if (!targetPart) {
				App::ConsolePrintF("No selected or hovered part found! Select or hover over a part and run this command again!");
				return;
			}

			customModeParts.erase(
				std::remove_if(customModeParts.begin(), customModeParts.end(),
					[targetPart](const CustomModeEntry& entry) {
						return entry.part == targetPart;
					}),
				customModeParts.end()
			);

			customModeParts.emplace_back(CustomModeEntry{ targetPart, currentMode });
			
			App::ConsolePrintF("Added selected/hovered part to custom stacking behavior.\nPart will now retain this stacking mode even when changing modes.");
		}
		else if (c == "remove" || c == "delete" || c == "erase") {
			EditorRigblockPtr targetPart = Editor.mpSelectedPart ? Editor.mpSelectedPart : Editor.mpActivePart;
			if (!targetPart) {
				App::ConsolePrintF("No selected or hovered part found! Select or hover over a part and run this command again!");
				return;
			}

			auto it = std::remove_if(customModeParts.begin(), customModeParts.end(),
				[targetPart](const ToggleStacker::CustomModeEntry& entry) {
					return entry.part == targetPart;
				});

			if (it != customModeParts.end()) {
				customModeParts.erase(it, customModeParts.end());
				App::ConsolePrintF("Removed selected/hovered part from custom stacking behavior.\nPart will no longer follow its own stacking mode.");
			}
			else {
				App::ConsolePrintF("Part does not have any custom stacking mode. Nothing changed.");
			}
		}
		else if (c == "reset" || c == "clear") {
			customModeParts.clear();
			App::ConsolePrintF("Restored all custom part stacking behaviors to default.\nAll parts will now follow the current stacking mode.");
		}
		else {
			App::ConsolePrintF("Unknown Togglestacker custom argument. Avaliable custom arguments: 'add / include', 'remove / delete / erase', 'reset / clear'.");
		}
	}

	if (mode == nullptr && custom == nullptr) {
		if (currentMode != Mode::DEFAULT)
		{
			currentMode = Mode::DEFAULT;
			App::ConsolePrintF("ToggleStacker deactivated, default behavior restored for all* parts.");
		}
		else
		{
			currentMode = Mode::STACK_ALL;
			App::ConsolePrintF("ToggleStacker activated, all* parts will now stack.");
		}
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