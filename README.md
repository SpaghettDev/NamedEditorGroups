# Named Editor Groups

A mod commissioned by [Rue](https://x.com/xRueOfficial).

A mod that allows you to abstract away IDs by giving them names!

<img src="./assets/TriggerPopup.png" width="720"/>

<img src="./assets/NamedIDsPopup.png" width="720"/>

## API

This mod exposes 2 types of APIs:

- Dynamically Linked API:
  - You `#include <spaghettdev.named-editor-groups/api/NIDManager.hpp>` and use the methods in the NIDManager namespace
- Events API:
  - Similar to TextureLoader's API, you can dispatch events to "spaghettdev.named-editor-groups/v1/[API-ID]" to request information
  - You must `#define NAMED_EDITOR_GROUPS_USE_EVENTS_API` before including the api header

## How to install (from Geode)

1. Get [Geode](https://geode-sdk.org)
2. Click the Geode button
3. Click the download tab, find Named Editor Groups, and click Install!

## How to install (from Releases)

1. Download the latest release's Named Editor Groups geode file
2. Copy it to `Geometry Dash/geode/mods`

## How to build

Run the following commands in your preferred terminal:

```sh
git clone https://github.com/SpaghettDev/NamedEditorGroups
cd NamedEditorGroups
mkdir build
cd build
cmake ..
```

Then open the generated solution in Visual Studio.
