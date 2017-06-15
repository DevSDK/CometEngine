using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CometEngine.Client
{
    class CometEngineDXRenderer
    {
        /// <summary>
        /// Initalize for CometEngin DirectX11 Renderer
        /// </summary>
        /// <param name="i_RenderWidth">Rendering Width</param>
        /// <param name="i_RenderHeight">Rendering Height</param>
        /// <param name="i_RenderScale">Rendering Scale</param>
        /// <param name="i_MSAAScale">MSAA Scale less 1 is No MSAA</param>
        /// <param name="IsFullScreen">DirectX FullScreen Flag</param>
        /// <returns></returns>
        [DllImport("CometEngineDXRenderer.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern bool Init(int i_RenderWidth, int i_RenderHeight, int i_RenderScale, int i_MSAAScale, bool IsFullScreen);
        
        /// <summary>
        /// DirectX11 Debug Device Enable
        /// </summary>
        /// <param name="i_Flag"></param>
        [DllImport("CometEngineDXRenderer.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void EnableDebugDevice(bool i_Flag);
        

    }
}
