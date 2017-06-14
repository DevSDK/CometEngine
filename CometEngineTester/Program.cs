using CELib.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace CometEngineTester
{
    class Program
    {
        static void Main(string[] args)
        {
            CometEngine.Launch("TITLE", 1200, 600, WIndowModes.DEFAULT, true, 4);
        }
    }
}
