using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace launcher
{
    internal class FileUtils
    {
        private static void RelocateToExecutable()
        {
            var curDir = Directory.GetCurrentDirectory();
            var exeDir = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            if (curDir != exeDir)
            {
                Directory.SetCurrentDirectory(exeDir);
                Console.WriteLine("Relocated to executable directory: " + exeDir);
            }
        }

        private static bool failed = false;
        public static string FindGameExecutable()
        {
            if (!failed)
            {
                RelocateToExecutable();
            }

            var curDir = Directory.GetCurrentDirectory();
            var exes = new Queue<string>(Directory.GetFiles(curDir, "*.exe"));

            while (exes.Count > 0 && exes.Peek().Contains("launcher.exe"))
            {
                exes.Dequeue();
            }
            if (exes.Count == 0)
            {
                if (failed)
                {
                    throw new Exception("Could not find the game executable!\nLooked in: " + curDir);
                }
                else
                {
                    // HACK: Use visual studio fallback
                    // Works on my machine.
                    failed = true;
                    Directory.SetCurrentDirectory("C:\\dev\\basalt\\x64\\Release");
                    return FindGameExecutable();
                }

            }
            return exes.Peek();
        }

    }
}
