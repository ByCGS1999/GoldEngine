Gold Engine, C++ MSCLR Game Engine.

Gold Engine is a Game Engine written with C++ under the MSCLR/CLI.

Features:</br>
  Editor [x]</br>
  Editor Customizable Style [x]</br>
  Playback Engine [~] - For now boots and loads the scene, needs more testing</br>
  Lighting [x]</br>
  Dynamic Lighting [x]</br>
  Global Illumination (GI) []</br>
  PBR [x]</br>
  Scripting API [x]</br>
  Assembly Management [x]</br>
  Physics Engine []</br>
  Asset Management & Packing [x]</br>
  Scene Management [x]</br>
  Embedded default objects [x]</br>
  Object Based System [x]</br>
  Entity Component System [] - NOR PLANNED</br>
  64 Bit Support [x] (you will need to compile assimp, raylib and bullet3 for x64 to make it run)</br>
  32 Bit Support [x] (you will need to compile assimp, raylib and bullet3 for x86 to make it run)</br>
  LuaVM [x] - Thanks To MoonSharp, it somehow works with C++/CLI<br>

Requirements:</br>
  C++ MSCLR.</br>
  VS2022 or higher. (2019 might work aswell)</br>
  .NET 8.0. (version can be tweaked)</br>
  A brain. (NOT OPTIONAL)</br>

Supported Platforms:</br>
    Windows [x]</br>
    Linux [X] - Tested with wine, works fine by using the WineBootstrapper application in this project. </br>
    MacOS [] ^ same, i guess.</br>
    Android [] - if you want to make a game for android with this you must be drunk or smth tbh.</br>

About Projects:</br>
    GoldEngine-Internal -> Entire game engine, from window management to low level asset management.</br>
    GoldEngine-Editor -> Example of scripts using the Scripting API. (Docs might come soon, idk.)</br>
    Utils/WineBootstrap/WineBootstrapper -> Tool that manages and automates wine bootstrapping and installation.
