using CometEngine.Client;
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
            CometEngineClient.Init("TITLE", 1200, 600, WIndowModes.DEFAULT, 1, 1);
        

            // TODO: Init Renderer 




            CometEngineClient.Launch();


        }
    }
}
