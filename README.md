# SFML Zombie Shooter

A top-down zombie shooter set in a dark, parallax-scrolling environment. The project is written in C++ and uses SFML for window management, input, sprite rendering, animation, and 2D transforms.

The player can move and aim independently, switch between four weapons, and fight a group of zombies that wander, detect, pursue, and attack the player. The game includes animated combat, collision detection, a tracking camera, a health and weapon HUD, and a complete menu-driven flow.

## Features

- Trackpad- and mouse-friendly pointer aiming
- Movement relative to the player's facing direction
- Flashlight, knife, pistol, and rifle weapon modes
- Animated idle, walking, melee, shooting, and enemy attack states
- Zombie wandering, vision cones, close-range detection, pursuit, and repeated attacks
- Bullet and melee collision detection
- Player health and damage cooldown system
- Weapon and health HUD
- Camera tracking and zoom controls
- Layered parallax environment with a night-time tint
- Title, story, main menu, pause menu, and credits screens
- Optional FPS display

## Controls

| Context | Key | Action |
| --- | --- | --- |
| Title/Menu | <kbd>Enter</kbd> | Continue or confirm the selected option |
| Menu | <kbd>Up</kbd> / <kbd>Down</kbd> | Select an option |
| Game | <kbd>W</kbd> / <kbd>A</kbd> / <kbd>S</kbd> / <kbd>D</kbd> | Move relative to the direction the player is facing |
| Game | Mouse or trackpad movement | Aim |
| Game | Primary click | Attack with the equipped weapon |
| Game | Secondary click | Perform a melee attack |
| Game | <kbd>1</kbd> | Equip the flashlight |
| Game | <kbd>2</kbd> | Equip the knife |
| Game | <kbd>3</kbd> | Equip the pistol |
| Game | <kbd>4</kbd> | Equip the rifle |
| Game | <kbd>+</kbd> / <kbd>-</kbd> | Zoom the camera in or out |
| Game | <kbd>F1</kbd> | Display FPS information while held |
| Story/Game | <kbd>Esc</kbd> | Skip the story or pause gameplay |

On a trackpad, a normal click performs the primary attack and a two-finger secondary click performs melee.

## Requirements

- Windows 10 or Windows 11
- Visual Studio 2022 with the **Desktop development with C++** workload
- MSVC v143 platform toolset
- Windows 10 SDK

The repository includes the required SFML 2.5.1 headers, libraries, and runtime DLLs under `ThirdParty/SFML-2.5.1`, so a separate SFML installation is not required for the supplied Visual Studio solution. The current project and build configuration target Windows.

## Building and running

1. Clone the repository:

   ```powershell
   git clone https://github.com/JackGore02/sfml-top-down-zombie-shooter.git
   cd sfml-top-down-zombie-shooter
   ```

2. Open `SFML Zombie Shooter.sln` in Visual Studio.
3. Select either **Debug** or **Release** and the **x64** platform.
4. Choose **Build > Build Solution** or press <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>B</kbd>.
5. Run without the debugger using <kbd>Ctrl</kbd>+<kbd>F5</kbd>.

You can also build from a Visual Studio Developer Command Prompt:

```powershell
msbuild "SFML Zombie Shooter.sln" /p:Configuration=Release /p:Platform=x64
```

The game loads media using paths relative to the `SFML Zombie Shooter` project directory. Running from Visual Studio provides the intended working directory. If the compiled executable is launched manually, set that project directory as the working directory so the textures, sprite data, fonts, and menu files can be found.

## Gameplay

- You begin with 100 health.
- Ten zombies are placed at random positions around the game world.
- Zombies wander until the player enters their vision cone or close-detection radius.
- An alerted zombie turns towards the player, pursues them, and attacks at close range.
- A successful zombie attack removes 25 health, with a short cooldown before the player can be damaged again.
- The flashlight and knife use primary-click melee attacks.
- The pistol and rifle fire projectiles with weapon-specific cooldowns.
- Secondary click performs a melee attack with any equipped weapon.
- Bullets disappear after hitting a zombie or reaching their maximum range.
- The HUD updates to show the equipped weapon and the player's remaining health.

## Project structure

```text
SFML Zombie Shooter.sln
|-- SFML Zombie Shooter/
|   |-- main.cpp                         # Application entry point
|   |-- SFML_Application.*               # Main loop and screen-state coordination
|   |-- SFML_GameWorld.*                 # Gameplay, collisions, HUD, and rendering
|   |-- SFML_2DCamera.*                  # Camera tracking, transforms, and zoom
|   |-- SFML_GameWorldLayer.*            # Parallax background layers
|   |-- SFML_SpriteObject.*              # Base renderable sprite object
|   |-- SFML_AnimatedSpriteObject.*      # Animated sprite playback
|   |-- SFML_SpriteAnimation.*           # Animation frames and timing
|   |-- SFML_PlayableSpriteObject.*      # Player input, movement, and weapons
|   |-- SFML_NPCSpriteObject.*           # Zombie awareness, movement, and attacks
|   |-- SFML_BulletSpriteObject.*        # Projectile movement and range
|   |-- SFML_TitleScreen.*               # Opening title screen
|   |-- SFML_GameMenu.*                  # Main menu
|   |-- SFML_StoryScreen.*               # Scrolling story screen
|   |-- SFML_PauseMenu.*                 # Pause menu
|   |-- SFML_CreditScreen.*              # Credits screen
|   `-- Media/                            # Textures, HUD assets, fonts, and sprite data
`-- ThirdParty/
    `-- SFML-2.5.1/                       # Bundled SFML development files
```

## How it works

`SFML_Application` owns the fixed-step main loop and switches between the title screen, main menu, story, gameplay, pause menu, credits, and exit states. It processes the complete SFML event queue, updates the active state at 60 updates per second, and renders the result to a fullscreen window.

`SFML_GameWorld` owns the player, zombies, bullets, camera, parallax layers, collision handling, and HUD. World objects are rendered through the camera's combined view-projection transform, while HUD elements are drawn in screen space. Background and foreground layers use different parallax factors to create depth.

Renderable game objects derive from `SFML_SpriteObject`. `SFML_AnimatedSpriteObject` adds frame-based animation through `SFML_SpriteAnimation`, while the playable character and zombies extend that system with their own action states. Zombies alternate between wandering, pursuing, and attacking according to distance and vision checks.

## Disclaimer

This project was developed as commissioned coursework for university assessment. The source code presented here is published solely for educational and portfolio purposes only. No permission is granted to reproduce, distribute, submit, or adapt it without prior written permission.

The credits section included in the game was a required part of the coursework specification. The people and entities named in that section did not participate in, contribute to, sponsor, or endorse this project. I was the sole developer of the project.
