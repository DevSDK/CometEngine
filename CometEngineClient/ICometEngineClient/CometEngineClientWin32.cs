using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CometEngine.Client
{
    static class CometEngineClientWin32
    {

        [DllImport("CometEngineWin32.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern bool Init(string TitleName, int width, int height, WIndowModes mode, IntPtr hInstance, string lpCmdLine);

        [DllImport("CometEngineWin32.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void Launch();



    }
}
