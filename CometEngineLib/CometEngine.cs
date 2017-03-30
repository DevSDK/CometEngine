using CELib.Core.Native;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CELib.Core
{
    public enum WIndowModes
    {
        DEFAULT = 0,
        NO_BORDER,
        FULL_SCREEN
    }

    public class CometEngine
    {
        public static void Launch(string WindowTitle, int Width, int Height, WIndowModes Displaymode, bool isMass, int RenderScale )
        {
            CometEngineNative.CometEngineWIn32Init(WindowTitle, Width, Height, Displaymode,
                Process.GetCurrentProcess().Handle /*hInstance*/,  string.Empty/*lpCmdLine*/); 

        } //TODO: Must be Update Parameters
        
    }

  

}
