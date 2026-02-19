# Named Editor Groups Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [2.4.0-beta] - 2026-02-07

## Changed

- Target Geode version (v5.0.0-beta.3)
- Optimimzed codebase
- Events

## Fixed

- Importing NIDs and exiting not dumping into level
- Crash when using build helper on many objects
- Touch priority issues on Android

## [2.3.3] - 2025-12-17

## Added

- Post event for Improved Group View

## Changed

- Base64 implementation to use geode::utils::base64

## [2.3.2] - 2025-12-04

## Fixed

- Custom Color Popup crash
- Multiple Spawn Group Trigger crash
- Android Build Helper/Regroup crash on specific triggers
- Object labels crashing if scrolling with too much velocity

## [2.3.1] - 2025-12-02

## Fixed

- NIDManager::getNameForID implementation
- Made API noexcept (fixed Improved Group View crash)

## [2.3.0] - 2025-12-01

## Added

- Show Preview Button setting
- Ability to Esc from Edit Named ID popups
- Color Trigger label support

## Changed

- Target Geode version (v4.10.0)
- No longer detect Better Color Picker mod

## Fixed

- Touch priority issue in Set Group ID popup (thanks @Fryy55)
- Crashes on Android (and hopefully Mac/iOS)
- Unnecessary struct copies (speeds up level loading)
- Area Anim Trigger Popup input bug

## [2.2.2] - 2025-11-03

## Added

- JIT-Less support for iOS

## [2.2.1] - 2025-11-02

## Fixed

- Fixed massive memory leak from autofill input nodes creating and leaking named cells on each textbox.
- Fixed autocomplete cell ID recreation on every input change, eliminating typing lag.

## [2.2.0] - 2025-05-21

## Added

- Autofill preview when typing a group's name
- Support for spawn remaps
- Named IDs popup in Edit Level Layer
- Missing Named IDs input for some triggers

## Fixed

- Better Color Picker support
- Importing base64 string error
- Various crashes
- Being able to unselect the selected filter

## [2.0.3] - 2025-04-21

## Added

- Copy channel ID Named ID input for color triggers

## Fixed

- Player objects having an ID stuck to them
- Better Color Picker receiving inputs when Edit Named ID popup is active
- Old color triggers crashing the game
- Unable to type symbols in Edit Named IDs popup

## [2.0.2] - 2025-04-19

## Fixed

- Crash on android when opening color IDs layer
- Crash when Level ID API isn't installed

## [2.0.1] - 2025-04-19

## Fixed

- Rare crash when updating Named ID preview
- Pulse Trigger not showing Named ID preview when target type is channel

## [2.0.0] - 2025-04-18

## Added

- Timer IDs
- Effect IDs
- Color IDs
- iOS support
- Build Helper support (name format can be modified in mod settings)
- Ability to set description and preview to Named IDs (needs [Level ID API by Cvolton](mod:cvolton.level-id-api))
- Import/Export Named IDs
- Shortcut to Named IDs popup in Set Group ID Layer
- SetIDPopup support (Find GroupID, GroupID filter, Color fiter in delete tab)
- Spawn Group ID input
- Named ID count in filter popup

## Changed

- Name length limit (20 -> 24)
- API version (v1 -> v2)
- Better error handling when parsing save object
- Save object location
- Name preview for Link Visibility trigger
- Increased performance of trigger popups setup
- Increased performance of object labels refresh

## Fixed

- Save object getting saved on the current editor layer instead of 0
- Selecting multiple objects would incorrectly highlight the wrong IDs
- Missing Named ID input for some triggers
- Counter Label trigger incorrect ID type (#12), thank you [FreakyRobot](https://github.com/MaSp005)
- Sequence Trigger crashing/adding wrong IDs

## [1.0.4] - 2025-03-01

### Fixed

- Lag when entering a level

## [1.0.3] - 2025-02-26

### Fixed

- Export event headers in API

## [1.0.2] - 2025-02-26

### Fixed

- Set Group ID crash when more than 1 object is selected (#6)

### Changed

- Events API

## [1.0.1] - 2025-02-25

### Changed

- Mod logo

### Fixed

- Advanced Random Trigger popup crash
- Sequence Trigger popup crash
- Collision Trigger popup crash (#5)
- Named IDs popup items weird positioning bug (#4)

## [1.0.0] - 2025-02-22

Initial Commit
