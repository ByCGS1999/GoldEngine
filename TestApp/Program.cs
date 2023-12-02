using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Engine.Drawing;
using Engine.Internal.Components;

namespace TestApp
{
    public enum CameraType
    {
        C3D,
        C2D
    }

    public enum CameraMode
    {
        CAMERA_CUSTOM = 0,              // Custom camera
        CAMERA_FREE,                    // Free camera
        CAMERA_ORBITAL,                 // Orbital camera
        CAMERA_FIRST_PERSON,            // First person camera
        CAMERA_THIRD_PERSON             // Third person camera
    }

    public enum ConfigFlags
    {
        FLAG_VSYNC_HINT = 0x00000040,   // Set to try enabling V-Sync on GPU
        FLAG_FULLSCREEN_MODE = 0x00000002,   // Set to run program in fullscreen
        FLAG_WINDOW_RESIZABLE = 0x00000004,   // Set to allow resizable window
        FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
        FLAG_WINDOW_HIDDEN = 0x00000080,   // Set to hide window
        FLAG_WINDOW_MINIMIZED = 0x00000200,   // Set to minimize window (iconify)
        FLAG_WINDOW_MAXIMIZED = 0x00000400,   // Set to maximize window (expanded to monitor)
        FLAG_WINDOW_UNFOCUSED = 0x00000800,   // Set to window non focused
        FLAG_WINDOW_TOPMOST = 0x00001000,   // Set to window always on top
        FLAG_WINDOW_ALWAYS_RUN = 0x00000100,   // Set to allow windows running while minimized
        FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
        FLAG_WINDOW_HIGHDPI = 0x00002000,   // Set to support HighDPI
        FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
        FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000, // Set to run program in borderless windowed mode
        FLAG_MSAA_4X_HINT = 0x00000020,   // Set to try enabling MSAA 4X
        FLAG_INTERLACED_HINT = 0x00010000    // Set to try enabling interlaced video format (for V3D)
    }

    public enum CameraProjection {
        CAMERA_PERSPECTIVE = 0,         // Perspective projection
        CAMERA_ORTHOGRAPHIC             // Orthographic projection
    }

    internal class Program : Engine.Window
    {
        [DllImport("kernel32.dll")]
        static extern bool FreeConsole();

        static void Main(string[] args)
        {
            new Program().Start();
        }

        public override void Exit()
        {
            Drawing.HL_FreeAll();
        }

        void Start()
        {
            #if RELEASE && PUBLISH_MODE
                FreeConsole();
            #endif
            SetWindowFlags((uint)(ConfigFlags.FLAG_WINDOW_RESIZABLE | ConfigFlags.FLAG_WINDOW_TOPMOST));
            OpenWindow(640, 480, "Raylib window");
            SetFPS(60);
            Engine.Scripting.ScriptSandbox sandbox = new Engine.Scripting.ScriptSandbox();
            sandbox.AddScript(new ScriptTest());
            
            Drawing.HL_LoadModel(0, "Data/Models/castle.obj");
            Drawing.HL_LoadShader(0, "Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/base.fs");
            Drawing.HL_CreateCamera(0, new Vector3(0, 0, 0), (int)CameraType.C3D);
            //Drawing.HL_CreateEnvironment("Data/Engine/Skybox/Default.hdr", 1024, 32, 256, 512);
            Drawing.HL_LoadTexture2D(0, "Data/Models/castle_diffuse.png");
            Drawing.HL_CreateMaterial(0);
            Drawing.HL_SetMaterialShader(0, 0);
            Drawing.HL_SetMaterialTexture(0, 0);
            Drawing.HL_SetModelTexture(0, 0);
            Drawing.HL_SetCameraFov(0, 60);
            Drawing.HL_SetCameraProjection(0, (int)CameraProjection.CAMERA_PERSPECTIVE);
            
            Drawing.HL_UpdateCamera(0, (int)CameraType.C3D, (int)CameraMode.CAMERA_ORBITAL);

            AddToDraw(() =>
            {
                Drawing.HL_BeginRendering();
                Drawing.HL_ClearBackground(0x000000);
                Drawing.HL_Begin3DMode(0);
                Drawing.HL_DrawTexture(0, 0, 0, 0xFFFFFF);
                Drawing.HL_DrawModel(0, new Vector3(0, 0, 1), 1, 0xFFFFFF);
                Drawing.HL_DrawGrid(10, 1.0f);
                //Drawing.HL_DrawSkybox(0, (int)CameraType.C3D);
                Drawing.HL_EndMode((int)CameraType.C3D);

                Drawing.HL_EndRendering();
            });

                
        }
    }

    public class ScriptTest : Engine.Scripting.ScriptBehaviour
    {
        public override void Start()
        {
            Console.WriteLine("Hello World!");
        }

        public override void FixedUpdate()
        {
            Console.WriteLine("Running this message every 60 frames");
        }
    }
}
