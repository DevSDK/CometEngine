using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CometEngine.Client
{
    public static class CometEngineClient
    {

        
        public static bool Init(string WindowTitle, int Width, int Height, WIndowModes DisplayMode, int RenderScale, int MsaaLevel)
        {
            bool SucessFlag =  CometEngineClientWin32.Init(WindowTitle, Width, Height, DisplayMode,
               Process.GetCurrentProcess().Handle /*hInstance*/, string.Empty/*lpCmdLine*/);
            if (SucessFlag == false)
               return false;

            CometEngineDXRenderer.EnableDebugDevice(true);
            SucessFlag = CometEngineDXRenderer.Init(Width, Height, RenderScale, MsaaLevel, (DisplayMode == WIndowModes.FULL_SCREEN));
            if (SucessFlag == false)
                return false;
         
            return true;
        }

        public static void Launch()
        {
            CometEngineClientWin32.Launch();
        }

    }
}
