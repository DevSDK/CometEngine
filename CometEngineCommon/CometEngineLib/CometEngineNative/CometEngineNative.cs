using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CELib.Core.Native
{

    class CometEngineNative
    {
        [DllImport("CometEngine.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern bool CometEngineWIn32Init(string TitleName, int width, int height, WIndowModes mode, IntPtr hInstance, string lpCmdLine);

        [DllImport("CometEngine.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void LanuchCometEngine();
    }
}
